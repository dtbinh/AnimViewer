/**
 * @file model.hpp
 * @brief Model class
 *
 * @author Eric Butler (edbutler)
 */

#ifndef _462_SCENE_MODEL_HPP_
#define _462_SCENE_MODEL_HPP_

#include "scene/bounding_box.hpp"
#include "scene/scene.hpp"
#include "scene/mesh.hpp"

namespace Luc {

class Ray;

/**
 * A mesh of triangles.
 */
class Model : public Geometry
{
public:

    const Mesh* mesh;
    const Material* material;
    BoundingBox m_bounding_box;

    Model();
    virtual ~Model();

    virtual void render() const;

    // ray casting algorithm, check if a given line will hit this geometry.
    // line is already in geomtry's coordinates
    virtual bool ray_casting(const Ray&      line,
                             const real_t    tMin, 
                             const real_t    tMax,
                             float&          t, 
                             HitVertexInfor& hit_vertex) ;

    /* build bounding box for this model.
     * This method only exist in Model, not in Sphere or Triangle, since they 
     * are too simple that need not to use bounding box to be optimized.
     */
    void build_bounding_box();
};


} /* Luc */

#endif /* _462_SCENE_MODEL_HPP_ */

