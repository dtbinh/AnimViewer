#include "lucPCH.h"
#include "raycasting.hpp"

namespace Luc{

/**
 * Ray casting  for triangle. 
 * This function is called by both Model and Triangle.
 *
 * @param[in]  line    The line object.
 * @param[in]  tMin    The minimum legal number of t.
 * @param[in]  tMax    The maximum legal number of t.
 * @param[out] t       Time the ray cost to hit this geometry.
 * @param[out] beta    Barycentric coordinates.
 * @param[out] gamma   Barycentric coordinates.
 * @return true if given line hit this geometry, otherwise false.
 */
bool ray_casting_triangle(const Ray& line,
                          const Vector3& p0, const Vector3& p1, const Vector3& p2,
                          const real_t tMin, const real_t tMax,
                          float& t,          float& beta,       float& gamma)
{
    // see Shirley's Fundamentals of Computer Graphics, page 208
    Vector3 edge1 = p0 - p1;
    Vector3 edge2 = p0 - p2;
    Vector3 vea   = p0 - line.Point();  // Pa - Pe
    Vector3 direction = line.Direction();
    float a = edge1.x;
    float b = edge1.y;
    float c = edge1.z;
    float d = edge2.x;
    float e = edge2.y;
    float f = edge2.z;
    float g = direction.x;
    float h = direction.y;
    float i = direction.z;
    float j = vea.x;
    float k = vea.y;
    float l = vea.z;

    Vector3 A(a, b, c);
    Vector3 B(e*i-h*f, g*f-d*i, d*h-e*g);
    Vector3 C(j, k, l);
    Vector3 D(i, h, g);
    Vector3 E(a*k-j*b, j*c-a*l, b*l-k*c);
    Vector3 F(f, e, d);

    float M     = dot(A, B);
    if (fabs(M) < 1e-20)
        return false;

    beta  = dot(C, B) / M;
    gamma = dot(D, E) / M;
    t     = -dot(F, E) / M;

    if (t < tMin || t > tMax)
        return false;
    if (gamma < 0 || gamma > 1)
        return false;
    if (beta < 0 || beta > 1-gamma)
        return false;
    return true;
}

Color3 interpolate_texture_color(Vector2 tex_coord, const Material* material)
{
    float x = tex_coord.x - int(tex_coord.x);
    float y = tex_coord.y - int(tex_coord.y);
    if (x<0) x = x + 1.0f;
    if (y<0) y = y + 1.0f;

    int tex_width  = material->get_texture_width();
    int tex_height = material->get_texture_height();
    float x_repeated = x * tex_width;
    float y_repeated = y * tex_height;
    int left    = int(x_repeated);
    int right   = left+1;
    int bottom  = int(y_repeated);
    int top     = bottom+1;
    // avoid the situation that left and bottom is on the boundary.
    if (right == tex_width)
        right = 0;
    if (top == tex_height)
        top = 0;

    Color3 lt_color = material->get_texture_pixel(left,  top);
    Color3 lb_color = material->get_texture_pixel(left,  bottom);
    Color3 rt_color = material->get_texture_pixel(right, top);
    Color3 rb_color = material->get_texture_pixel(right, bottom);

    Color3 tex_color = bilinear_interpolation( lt_color, lb_color, rt_color, rb_color, 
                                               x_repeated-left, y_repeated-bottom );
    return tex_color;
}


} // namespace Luc

