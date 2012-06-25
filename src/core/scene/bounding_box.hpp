#pragma once
#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "math/vector.hpp"
#include "math/ray.hpp"

namespace Luc{

template<class T>
void swap(T& t1, T&t2)
{
    T t = t1;
    t1 = t2;
    t2 = t;
}

/*
 * A bounding box, used to detect if a ray will hit the model in this box.
 */
class BoundingBox
{
public:
    BoundingBox() : left_bottom_front_vertex(1000000000, 100000000, 10000000), 
                    right_top_back_vertex(-1000000000, -100000000, -10000000) {}

    /*
     * check if this bounding box is empty.
     * If this bounding box's volume is zero, it is empty.
     */
    bool IsEmpty()
    {
        return (left_bottom_front_vertex == Vector3(1000000000, 100000000, 10000000) && 
                right_top_back_vertex == Vector3(-1000000000, -100000000, -10000000));
    }

    
    /*
     * filter one vertex to enlarge this bounding box.
     * \@param vertex A vertex which need to be filtered.
     */
    void filter_vertex(const Vector3& vertex)
    {
        left_bottom_front_vertex = vmin(left_bottom_front_vertex, vertex);
        right_top_back_vertex    = vmax(right_top_back_vertex   , vertex);
    }

    /*
     * Get eight corner vertices of this bounding box
     */
    Vector3 get_left_top_front_corner() 
    { 
        return Vector3( left_bottom_front_vertex.x, 
                        right_top_back_vertex.y, 
                        left_bottom_front_vertex.z );
    }

    Vector3 get_right_top_front_corner()
    {
        return Vector3( right_top_back_vertex.x, 
                        right_top_back_vertex.y, 
                        left_bottom_front_vertex.z );
    }

    Vector3 get_left_bottom_front_corner()
    {
        return left_bottom_front_vertex;
    }

    Vector3 get_right_bottom_front_corner()
    {
        return Vector3( right_top_back_vertex.x, 
                        left_bottom_front_vertex.y, 
                        left_bottom_front_vertex.z );
    }

    Vector3 get_left_top_back_corner()
    {
        return Vector3( left_bottom_front_vertex.x, 
                        right_top_back_vertex.y, 
                        right_top_back_vertex.z );
    }

    Vector3 get_right_top_back_corner()
    {
        return right_top_back_vertex;
    }

    Vector3 get_left_bottom_back_corner()
    {
        return Vector3( left_bottom_front_vertex.x, 
                        left_bottom_front_vertex.y, 
                        right_top_back_vertex.z );
    }

    Vector3 get_right_bottom_back_corner()
    {
        return Vector3( right_top_back_vertex.x, 
                        left_bottom_front_vertex.y, 
                        right_top_back_vertex.z );
    }

    /*
     * get min/max x, y, z value.
     */
    float min_x() const { return left_bottom_front_vertex.x; }
    float min_y() const { return left_bottom_front_vertex.y; }
    float min_z() const { return left_bottom_front_vertex.z; }
    float max_x() const { return right_top_back_vertex.x; }
    float max_y() const { return right_top_back_vertex.y; }
    float max_z() const { return right_top_back_vertex.z; }


    /**
     * ray casting algorithm, check if a given line will hit this geometry.
     * line is already in geomtry's coordinates.
     *
     * @param[in]  ray          The ray object.
     * @param[in]  tMin         The minimum legal number of t.
     * @param[in]  tMax         The maximum legal number of t.
     * @param[out] t            Time the ray cost to hit this geometry.
     * @return true if given line hit this geometry, otherwise false.
     */
    virtual bool ray_casting(const Ray&      ray,
                             const real_t    tMin, 
                             const real_t    tMax,
                             float&          t);
private:
    Vector3 left_bottom_front_vertex;
    Vector3 right_top_back_vertex;    
}; // end of class BoundingBox

} // end of namespace Luc

#endif //BOUNDING_BOX_H
