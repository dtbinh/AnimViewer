/**
 * @file sphere.cpp
 * @brief Function defnitions for the Sphere class.
 *
 * @author Kristin Siu (kasiu)
 * @author Eric Butler (edbutler)
 */
#include "lucPCH.h"
#include "app/raycasting.hpp"
#include "scene/sphere.hpp"
#include "application/opengl.hpp"
#include "math/ray.hpp"
#include "math/vector.hpp"

#include <cmath>

namespace Luc {

#define SPHERE_NUM_LAT 80
#define SPHERE_NUM_LON 100

#define SPHERE_NUM_VERTICES ( ( SPHERE_NUM_LAT + 1 ) * ( SPHERE_NUM_LON + 1 ) )
#define SPHERE_NUM_INDICES ( 6 * SPHERE_NUM_LAT * SPHERE_NUM_LON )
// index of the x,y sphere where x is lat and y is lon
#define SINDEX(x,y) ((x) * (SPHERE_NUM_LON + 1) + (y))
#define VERTEX_SIZE 8
#define TCOORD_OFFSET 0
#define NORMAL_OFFSET 2
#define VERTEX_OFFSET 5

static unsigned int Indices[SPHERE_NUM_INDICES];
static float Vertices[VERTEX_SIZE * SPHERE_NUM_VERTICES];

static void init_sphere()
{
    static bool initialized = false;
    if ( initialized )
        return;

    for ( int i = 0; i <= SPHERE_NUM_LAT; i++ ) {
        for ( int j = 0; j <= SPHERE_NUM_LON; j++ ) {
            real_t lat = real_t( i ) / SPHERE_NUM_LAT;
            real_t lon = real_t( j ) / SPHERE_NUM_LON;
            float* vptr = &Vertices[VERTEX_SIZE * SINDEX(i,j)];

            vptr[TCOORD_OFFSET + 0] = lon;
            vptr[TCOORD_OFFSET + 1] = 1-lat;

            lat *= PI;
            lon *= 2 * PI;
            real_t sinlat = sin( lat );

            vptr[NORMAL_OFFSET + 0] = vptr[VERTEX_OFFSET + 0] = sinlat * sin( lon );
            vptr[NORMAL_OFFSET + 1] = vptr[VERTEX_OFFSET + 1] = cos( lat ),
            vptr[NORMAL_OFFSET + 2] = vptr[VERTEX_OFFSET + 2] = sinlat * cos( lon );
        }
    }

    for ( int i = 0; i < SPHERE_NUM_LAT; i++ ) {
        for ( int j = 0; j < SPHERE_NUM_LON; j++ ) {
            unsigned int* iptr = &Indices[6 * ( SPHERE_NUM_LON * i + j )];

            unsigned int i00 = SINDEX(i,  j  );
            unsigned int i10 = SINDEX(i+1,j  );
            unsigned int i11 = SINDEX(i+1,j+1);
            unsigned int i01 = SINDEX(i,  j+1);

            iptr[0] = i00;
            iptr[1] = i10;
            iptr[2] = i11;
            iptr[3] = i11;
            iptr[4] = i01;
            iptr[5] = i00;
        }
    }

    initialized = true;
}

Sphere::Sphere()
    : radius(0), material(0) 
{
    m_bNoTransformationMatrix = true;
}

Sphere::~Sphere() {}

void Sphere::render() const
{
    // create geometry if we haven't already
    init_sphere();

    if ( material )
        material->set_gl_state();

    // just scale by radius and draw unit sphere
    glPushMatrix();
    glScaled( radius, radius, radius );
    glInterleavedArrays( GL_T2F_N3F_V3F, VERTEX_SIZE * sizeof Vertices[0], Vertices );
    glDrawElements( GL_TRIANGLES, SPHERE_NUM_INDICES, GL_UNSIGNED_INT, Indices );
    glPopMatrix();

    if ( material )
        material->reset_gl_state();
}

// ray casting algorithm, check if a given line will hit this geometry.
// line is already in geomtry's coordinates
bool Sphere::ray_casting(const Ray&      line,
                         const real_t    tMin, 
                         const real_t    tMax,
                         float&          t, 
                         HitVertexInfor& hit_vertex) 
{
    // Originally, the sphere is sphere = T*R*S*original_sphere, line = original_ray.
    // In order to compute easily, we transform to unit sphere space to cast ray,
    // so sphere = original_sphere, line = invS*invR*invT*original_ray
    // 
    // e = invS*invR*invT*original_e, d = invS*invR*original_d

    // transform ray to geometry's coordinates
    Vector3 e = (m_invTransformMat * Vector4(line.Point(), 1)).xyz();
    Vector3 d = (m_invTransformMatWithoutTranslation * Vector4(line.Direction(), 1)).xyz();
    Vector3 c(0, 0, 0);
    real_t  r = this->radius;

    // B=dot(2d, e-c), A=dot(d,d), C=dot(e-c, e-c)-r^2
    // discriminant = B^2-4*A*C = (dot(d, e-c))^2 - dot(d,d)*(dot(e-c,e-c)-r^2)
    Vector3 vce = e-c;          // vector from c to e
    real_t dMCde = dot(d, vce); // dot(d, e-c), Distance from Pid point of CorD to E
    real_t discriminant = dMCde * dMCde - dot(d, d) * (dot(vce, vce) - r*r);

    // there is no hit at all
    if (discriminant < 0)
        return false;

    // hit
    real_t dLength = dot(d, d);
    real_t t1 = (-dMCde + sqrt(discriminant)) / dLength;
    real_t t2 = (-dMCde - sqrt(discriminant)) / dLength;

    t1 = t1<tMin ? tMax+1 : t1;
    t2 = t2<tMin ? tMax+1 : t2;
    t  = t1<t2   ? t1     : t2;
    if (t > tMax)
        return false;
    
    Vector3 hit_sphere_position = e + t * d;
    // compute hit vertex information 
    hit_vertex.position     = hit_sphere_position; // in local coordinates
    hit_vertex.normal       = hit_vertex.position - c;
    hit_vertex.ambient      = material->ambient;
    hit_vertex.diffuse      = material->diffuse;
    hit_vertex.specular     = material->specular;
    hit_vertex.refractive_index = material->refractive_index;

    // transform to global coordinates
    hit_vertex.position     = (m_transformatMat * Vector4(hit_vertex.position, 1)).xyz(); // in global coordinates
    hit_vertex.normal       = normalize(m_normalMatrix * hit_vertex.normal); // in global coordinates

    // texture color
    static const float pi2 = 2 * PI;
    float tx = atan2(hit_sphere_position.x, hit_sphere_position.z);
    if (tx < 0)
        tx += pi2;
    tx /= pi2;
    float ty = (PI - acos(hit_sphere_position.y / radius)) / pi2;
    Vector2 tex_coord(tx, ty);// texture coordinates
    hit_vertex.tex_color = interpolate_texture_color(tex_coord, material);

    return true;
}

} /* Luc */

