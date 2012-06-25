/**
 * @file raytacer.cpp
 * @brief Raytracer class
 *
 * Implement these functions for project 2.
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */
#include "lucPCH.h"
#include "hit_vertex_infor.hpp"
#include "math/ray.hpp"
#include "raytracer.hpp"
#include "scene/scene.hpp"
#include "math/camera.hpp"

#include <SDL/SDL_timer.h>
#include <iostream>
#include <time.h>
#include <cmath>
#include <float.h>


namespace Luc {

Raytracer::Raytracer()
: scene( 0 ), width( 0 ), height( 0 ), SLOPE_FACTOR(FLT_MIN) { }

Raytracer::~Raytracer() { }

/**
 * Initializes the raytracer for the given scene. Overrides any previous
 * initializations. May be invoked before a previous raytrace completes.
 * @param scene The scene to raytrace.
 * @param width The width of the image being raytraced.
 * @param height The height of the image being raytraced.
 * @return true on success, false on error. The raytrace will abort if
 *  false is returned.
 */
bool Raytracer::initialize( Scene* scene, size_t width, size_t height, Camera& camera )
{
    this->scene = scene;
    this->width = width;
    this->height = height;

    current_row = 0;
    real_t fWidth  = static_cast<real_t>(width);
    real_t fHeight = static_cast<real_t>(height);

    // TODO any initialization or precompuation before the trace
    // Spatial Data Structure implementation here

    // get near clip plane and far clip plane, up and right step vector
    real_t near_clip = camera.GetNearClip();
    real_t far_clip  = camera.GetFarClip();
    real_t fov       = camera.GetFovRadians();
    real_t near_clip_height = near_clip * tan(fov/2);
    real_t near_clip_width  = near_clip_height * fWidth / fHeight;

    m_camera_pos   = camera.GetPosition();
    m_camera_dir  = camera.GetDirection() * near_clip;
    Vector3 unit_up    = normalize(camera.GetUp());
    Vector3 unit_right = normalize(cross(m_camera_dir, unit_up));
    m_up_step    = unit_up * near_clip_height / fHeight * 2;
    m_right_step = unit_right * near_clip_width / fWidth * 2;

    m_near_clip = near_clip;
    m_far_clip = far_clip;

    // build inverse transformation matrix for all geometries
    Geometry* const* geometries = scene->get_geometries();
    size_t geometry_count = scene->num_geometries();
    for (size_t i=0; i<geometry_count; i++)
    {
        Geometry* pGeom = geometries[i];
        pGeom->build_inverse_transformation_matrix();
    }

    return true;
}

/*
 * Detect if a ray will hit any geometry in legal time cost range. If hit 
 * any geometry, output intersection point information.
 * 
 * @param scene             The scene object, which contains all geometries 
 *                          information.
 * @param ray_dir           Direction vector of ray.
 * @param ray_pos           Start point of ray.
 * @param tMin              Minimum legal time cost for this ray.
 * @param tMax              Maximum legal time cost for this ray.
 * @param hit_vertex[out]   A struct storing all useful intersection point 
 *                          information.
 *
 * @return true if hit any geometry, otherwise false.
 */
bool Raytracer::ray_hit(Scene const*scene,                                    // geometries
                        const Vector3 &direction, const Vector3 &position,    // ray
                        const float tMin,         const float tMax,           // ray range
                        HitVertexInfor& hit_vertex  // intersection point information)
                       )
{
    // possible closest hit point information
    float closest_t = tMax+1;

    // for each geometry, test hit
    Geometry* const* geometries = scene->get_geometries();
    size_t geometry_count = scene->num_geometries();
    for (size_t i=0; i<geometry_count; i++)
    {
        Geometry* pGeom = geometries[i];

        Ray ray(position, direction);

        // current hit point information
        float cur_t;
        HitVertexInfor cur_hit_vertex;
        // if this ray hit a geometry, compare t
        if ( pGeom->ray_casting(ray, tMin, tMax, cur_t, cur_hit_vertex) )
        {
            if (cur_t < closest_t)
            {
                closest_t = cur_t;
                hit_vertex = cur_hit_vertex;
            } // end of is closer
        } // end of if hit
    } // end of all geometry loop

    if (closest_t < tMax) // hit a geometry
        return true;
    return false;           // if not hit any geometry
}

/* 
 * Calculate direct illumination light.
 *
 * @param scene         The scene object, which contains all geometries 
 *                      information.
 * @param hit_vertex    A struct storing all useful intersection point 
 *                      information.
 *
 * @return Direct illumination color.
 */
Color3 Raytracer::calculate_DI_light(Scene const*scene, 
                                     const HitVertexInfor &hit_vertex)
{
    // ambient light
    Color3 ambient_light = hit_vertex.ambient * scene->ambient_light;

    // accumulate all point light diffuse components
    Color3 diffuse_light(0, 0, 0);
    const PointLight* pPointLights = scene->get_lights();
    for (size_t i=0; i<scene->num_lights(); i++)
    {
        // point light
        PointLight point_light = pPointLights[i];

        // ray from hit point to light
        Vector3 shadow_ray_pos = hit_vertex.position;
        Vector3 shadow_ray_dir = point_light.position - shadow_ray_pos;
        real_t distance = length(shadow_ray_dir);
        shadow_ray_dir = normalize(shadow_ray_dir);

        // shadow ray hit test
        HitVertexInfor tmp_hit_vertex; // temp variable, didn't use it actually
        bool bExistObstacle = ray_hit(scene, shadow_ray_dir, shadow_ray_pos,
            distance/1000000, distance, tmp_hit_vertex);

        // if did not hit other geometry, accumulate diffuse light
        if (!bExistObstacle)
        {
            diffuse_light += point_light.get_attenuation_color(distance) *
                             hit_vertex.diffuse *
                             std::max(dot(hit_vertex.normal, shadow_ray_dir), 0.0f);
        }
    }

    // final direct illumination is multiplication of texture color and diffuse light
    return hit_vertex.tex_color * (ambient_light + diffuse_light);
}

/*
 * Check if this ray will be refracted. If refract, calculate direction of 
 * refracted ray and Fresnel coefficient.
 *
 * @param scene             The scene object, which contains all geometries 
 *                          information.
 * @param ray_dir           Direction vector of ray.
 * @param hit_vertex[out]   A struct storing all useful intersection point 
 *                          information.
 * @param rfr_ray_dir[out]  Direction of refracted ray.
 * @param R[out]            Fresnel coefficient.
 *
 * @return true if this ray will be refracted; otherwise false.
 *
 * @see Shirley's Fundamentals of Computer Graphics, page 215-216
 */
bool Raytracer::refraction_happened(Scene   const*  scene, 
                                    Vector3 const&  ray_dir, 
                                    HitVertexInfor& hit_vertex, 
                                    Vector3&        rfr_ray_dir,
                                    float&          R)
{
    Vector3 normal = hit_vertex.normal;
    real_t dn = dot(ray_dir, normal);
    real_t c;
    real_t nt;
    
    if (dn < 0) // the case ray go from air into the object.
    {
        nt = hit_vertex.refractive_index;
        real_t ri_ratio = scene->refractive_index / hit_vertex.refractive_index;
        rfr_ray_dir = ri_ratio * (ray_dir - normal*dn) - 
                            normal * sqrt(1 - ri_ratio*ri_ratio*(1-dn*dn));
        c = -dn;
    }
    else // the case ray go from object into air
    {
        normal = -normal;
        nt = scene->refractive_index;
        real_t ri_ratio = hit_vertex.refractive_index / scene->refractive_index;
        dn = dot(ray_dir, normal);
        real_t discriminant = 1 - ri_ratio*ri_ratio*(1-dn*dn);
        if (discriminant < 0)
        {
            R = 1;
            return false;
        }
        rfr_ray_dir = ri_ratio * (ray_dir - normal*dn) - 
                            normal * sqrt(discriminant);
        c = dot(rfr_ray_dir, -normal);
    }
    // Fresnel coefficient
    real_t SQRT_R0 = (nt-1)/(nt+1);
    real_t R0 = SQRT_R0 * SQRT_R0;
    real_t sup_c = 1-c;
    R = R0 + (1-R0) * sup_c * sup_c * sup_c * sup_c * sup_c;

    return true;
    
}

/*
 * Trace a ray, calculating the color of this ray.
 * This function is called recursively for recursive light.
 *
 * @param scene     The scene object, which contains all geometries information.
 * @param recursion The level of recursive light. If the level is larger or 
 *                  equal to 4, stop recursion.
 * @param ray_dir   Direction vector of ray.
 * @param ray_pos   Start point of ray.
 * @param tMin      Minimum legal time cost for this ray.
 * @param tMax      Maximum legal time cost for this ray.
 *
 * @return  The color of this ray.
 */
Color3 Raytracer::trace_ray(Scene const*scene,      // geometries
                            const int recursion,    // recursion level
                            const Vector3 &ray_dir, const Vector3 &ray_pos, // ray
                            const float tMin,       const float tMax        // ray range
                           )
{
    // if this function go beyond the last recursive level, stop and return black color.
    if (recursion <= 0)
        return Color3(0, 0, 0);

    // intersection point information
    HitVertexInfor hit_vertex;

    // if not hit any geometry, return background color
    bool bHit = ray_hit(scene, ray_dir, ray_pos, tMin, tMax, hit_vertex);
    if (!bHit)
        return scene->background_color;

    // if hit, compute color and return it
    Color3 DI_light (0, 0, 0);
    Color3 reflected_light(0, 0, 0);
    Color3 refracted_light(0, 0, 0);

    // 1. direct illumination
    if (hit_vertex.refractive_index == 0)
        DI_light = calculate_DI_light(scene, hit_vertex);

    // 2. reflection light
    if (hit_vertex.specular != Color3(0, 0, 0)) // avoid non-necessary reflection calculation
    {
        // reflection ray direction
        Vector3 rfl_ray_dir = normalize(
            ray_dir - 2 * dot(ray_dir, hit_vertex.normal) * hit_vertex.normal);

        reflected_light = hit_vertex.specular * hit_vertex.tex_color *
                          trace_ray(scene, recursion-1, rfl_ray_dir, hit_vertex.position,
                                    SLOPE_FACTOR, 1000000);
    }

    if (hit_vertex.refractive_index == 0)   // avoid non-necessary refraction calculation
        return DI_light + reflected_light;

    // 3. refraction light
    Vector3 rfr_ray_dir; // refractive ray direction
    float R;
    if (refraction_happened(scene, ray_dir, hit_vertex, rfr_ray_dir, R))
        refracted_light = hit_vertex.tex_color *
                          trace_ray(scene, recursion-1, rfr_ray_dir, hit_vertex.position, 
                                    SLOPE_FACTOR, 1000000);

    return DI_light + R * reflected_light + (1-R) * refracted_light;
}

/**
 * Performs a raytrace on the given pixel on the current scene.
 * The pixel is relative to the bottom-left corner of the image.
 * @param scene The scene to trace.
 * @param x The x-coordinate of the pixel to trace.
 * @param y The y-coordinate of the pixel to trace.
 * @param width The width of the screen in pixels.
 * @param height The height of the screen in pixels.
 * @return The color of that pixel in the final image.
 */
Color3 Raytracer::trace_pixel( const Scene* scene, size_t x, size_t y, size_t width, size_t height )
{
    assert( 0 <= x && x < width );
    assert( 0 <= y && y < height );

    // get ray direction
    Vector3 position  = m_camera_pos;
    Vector3 direction = m_camera_dir +                    // camera direction
                        m_up_step    * (y*1.0f-height/2) + // vertical increasement
                        m_right_step * (x*1.0f-width /2);  // horizontal increasement
    direction = normalize(direction);

    // trace a ray and return its color
    return trace_ray( scene, 4, direction, position, m_near_clip, m_far_clip );
}

/**
 * Raytraces some portion of the scene. Should raytrace for about
 * max_time duration and then return, even if the raytrace is not copmlete.
 * The results should be placed in the given buffer.
 * @param buffer The buffer into which to place the color data. It is
 *  32-bit RGBA (4 bytes per pixel), in row-major order.
 * @param max_time, If non-null, the maximum suggested time this
 *  function raytrace before returning, in seconds. If null, the raytrace
 *  should run to completion.
 * @return true if the raytrace is complete, false if there is more
 *  work to be done.
 */
bool Raytracer::raytrace( unsigned char *buffer, real_t* max_time )
{
    // TODO Add any modifications to this algorithm, if needed.

    static long start_time; // used to show how long time ray tracing cost
    if (0 == current_row)
        start_time = clock();

    static const size_t PRINT_INTERVAL = 64;

    // the time in milliseconds that we should stop
    unsigned int end_time = 0;
    bool is_done = false;

    if ( max_time ) {
        // convert duration to milliseconds
        unsigned int duration = (unsigned int) ( *max_time * 1000 );
        end_time = SDL_GetTicks() + duration;
    }

    // until time is up, run the raytrace. we render an entire row at once
    // for simplicity and efficiency.
    for ( ; !max_time || end_time > SDL_GetTicks(); ++current_row ) {

        if ( current_row % PRINT_INTERVAL == 0 ) {
            printf( "Raytracing (row %u)...\n", current_row );
        }

        // we're done if we finish the last row
        is_done = current_row == height;
        // break if we finish
        if ( is_done )
            break;

        for ( size_t x = 0; x < width; ++x ) {
            // trace a pixel
            Color3 color = trace_pixel( scene, x, current_row, width, height );
            // write the result to the buffer, always use 1.0 as the alpha
            color.to_array( &buffer[4 * ( current_row * width + x )] );
        }
    }

    if ( is_done ) {
        printf( "Done raytracing!\n" );
        printf( "Used %d milliseconds.\n", clock()-start_time );
    }


    return is_done;
}

} 

