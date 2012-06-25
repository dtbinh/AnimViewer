/**
 * @file raytacer.hpp
 * @brief Raytracer class
 *
 * Implement these functions for project 2.
 *
 * @author H. Q. Bovik (hqbovik)
 * @bug Unimplemented
 */

#ifndef _462_RAYTRACER_HPP_
#define _462_RAYTRACER_HPP_

#include "math/color.hpp"
#include "math/vector.hpp"

namespace Luc {

class Scene;
class Camera;
struct MeshVertex;
class Geometry;
struct HitVertexInfor;

class Raytracer
{
public:

    Raytracer();
    ~Raytracer();

    bool initialize( Scene* scene, size_t width, size_t height, Camera& camera );
    bool raytrace( unsigned char* buffer, real_t* max_time );

private:

    Color3 trace_pixel( const Scene* scene, 
                        size_t x, size_t y, 
                        size_t width, size_t height );

    /*
     * Trace a ray, calculating the color of this ray.
     * This function is called recursively for recursive light.
     *
     * @param scene     The scene object, which contains all geometries 
     *                  information.
     * @param recursion The level of recursive light. If the level is larger or 
     *                  equal to 4, stop recursion.
     * @param ray_dir   Direction vector of ray.
     * @param ray_pos   Start point of ray.
     * @param tMin      Minimum legal time cost for this ray.
     * @param tMax      Maximum legal time cost for this ray.
     *
     * @return  The color of this ray.
     */
    Color3 trace_ray( Scene const* scene,   // geometries   
                      const int recursion,  // recursion level
                      const Vector3 &ray_dir, const Vector3 &ray_pos,  // ray
                      const float tMin,       const float tMax         // ray range
                      );
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
    bool refraction_happened(Scene const*    scene, 
                             Vector3 const&  ray_dir, 
                             HitVertexInfor& hit_vertex, 
                             Vector3&        rfr_ray_dir,
                             float&          R);

    /* 
     * Calculate direct illumination light.
     *
     * @param scene             The scene object, which contains all geometries 
     *                          information.
     * @param hit_vertex[out]   A struct storing all useful intersection point 
     *                          information.
     *
     * @return Direct illumination color.
     */
    Color3 calculate_DI_light(Scene const*scene, const HitVertexInfor &hit_vertex);


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
    bool ray_hit( Scene const*scene,                                    // geometries
                  const Vector3 &ray_dir,   const Vector3 &ray_pos,     // ray
                  const float tMin,         const float tMax,           // ray range
                  HitVertexInfor& hit_vertex);  // intersection point information 

    // the scene to trace
    Scene* scene;

    // the dimensions of the image to trace
    size_t width, height;

    // the next row to raytrace
    size_t current_row;

    // camera properties, used to calculate eye ray equation
    Vector3 m_camera_pos;   // line = eye + t * d
    Vector3 m_camera_dir;   // direction
    Vector3 m_up_step;      // up step for each pixel
    Vector3 m_right_step;   // right step for each pixel
    real_t m_near_clip;      // minimum t when direction is unit vector
    real_t m_far_clip;       // maximum t when direction is unit vector

    // const slope factor
    const float SLOPE_FACTOR; /* = 1e-5 */

private:
    Raytracer& operator=(const Raytracer&) {};
};

} 

#endif /* _462_RAYTRACER_HPP_ */

