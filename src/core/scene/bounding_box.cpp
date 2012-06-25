#include "lucPCH.h"
#include "bounding_box.hpp"


namespace Luc{

/**
 * ray casting algorithm, check if a given ray will hit this geometry.
 * ray is already in geomtry's coordinates.
 *
 * @param[in]  ray          The ray object.
 * @param[in]  tMin         The minimum legal number of t.
 * @param[in]  tMax         The maximum legal number of t.
 * @param[out] t            Time the ray cost to hit this geometry.
 * @return true if given ray hit this geometry, otherwise false.
 */
bool BoundingBox::ray_casting( const Ray&      ray,
                               const real_t    tMin, 
                               const real_t    tMax,
                               float&          t )
{
    Vector3 ray_pos = ray.Point();
    Vector3 ray_dir = ray.Direction();

    // if ray is parallel with any axis, judge directly
    if (ray_dir.x == 0)
        if (ray_pos.x < max_x() && ray_pos.x > min_x())
            return true;
        else
            return false;
    if (ray_dir.y == 0)
        if (ray_pos.y < max_y() && ray_pos.y > min_y())
            return true;
        else
            return false;
    if (ray_dir.z == 0)
        if (ray_pos.z < max_z() && ray_pos.z > min_z())
            return true;
        else
            return false;

    float txmin, txmax, tymin, tymax, tzmin, tzmax;
    if (ray_dir.x >= 0)
    {
        txmin = (min_x() - ray_pos.x)/ray_dir.x;
        txmax = (max_x() - ray_pos.x)/ray_dir.x;
    }
    else
    {
        txmin = (max_x() - ray_pos.x)/ray_dir.x;
        txmax = (min_x() - ray_pos.x)/ray_dir.x;
    }
    if (ray_dir.y >= 0)
    {
        tymin = (min_y() - ray_pos.y)/ray_dir.y;
        tymax = (max_y() - ray_pos.y)/ray_dir.y;
    }
    else
    {
        tymin = (max_y() - ray_pos.y)/ray_dir.y;
        tymax = (min_y() - ray_pos.y)/ray_dir.y;
    }
    if (ray_dir.z >= 0)
    {
        tzmin = (min_z() - ray_pos.z)/ray_dir.z;
        tzmax = (max_z() - ray_pos.z)/ray_dir.z;
    }
    else
    {
        tzmin = (max_z() - ray_pos.z)/ray_dir.z;
        tzmax = (min_z() - ray_pos.z)/ray_dir.z;
    }

    if (txmin > tymax || tymin > txmax)
        return false;
    if (txmin > tzmax || tzmin > txmax)
        return false;
    if (tymin > tzmax || tzmin > tymax)
        return false;
    return true;
}


} // end of namespace Luc
