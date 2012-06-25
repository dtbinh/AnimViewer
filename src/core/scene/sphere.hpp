/**
 * @file sphere.hpp
 * @brief Class defnition for Sphere.
 *
 * @author Kristin Siu (kasiu)
 * @author Eric Butler (edbutler)
 */

#ifndef _462_SCENE_SPHERE_HPP_
#define _462_SCENE_SPHERE_HPP_

#include "scene/scene.hpp"
#include "math/matrix.hpp"

namespace Luc {

class Ray;

/**
 * A sphere, centered on its position with a certain radius.
 */
class Sphere : public Geometry
{
public:

    real_t radius;
    const Material* material;


    Sphere();
    virtual ~Sphere();
    virtual void render() const;

    // ray casting algorithm, check if a given line will hit this geometry.
    // line is already in geomtry's coordinates
    virtual bool ray_casting(const Ray&      line,
                             const real_t    tMin, 
                             const real_t    tMax,
                             float&          t, 
                             HitVertexInfor& hit_vertex) ;

};

} /* Luc */

#endif /* _462_SCENE_SPHERE_HPP_ */

