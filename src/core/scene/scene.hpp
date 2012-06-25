/**
 * @file scene.hpp
 * @brief Class definitions for scenes.
 *
 * @author Eric Butler (edbutler)
 * @author Kristin Siu (kasiu)
 */

#ifndef _462_SCENE_SCENE_HPP_
#define _462_SCENE_SCENE_HPP_

#include "app/hit_vertex_infor.hpp"
#include "math/vector.hpp"
#include "math/quaternion.hpp"
#include "math/matrix.hpp"
#include "math/camera.hpp"
#include "scene/material.hpp"
#include "scene/mesh.hpp"

namespace Luc {

class Ray;

class Geometry
{
public:
    Geometry();
    virtual ~Geometry();


    /*
       World transformation are applied in the following order:
       1. Scale
       2. Orientation
       3. Position
    */

    // The world position of the object.
    Vector3 position;

    // The world orientation of the object.
    // Use Quaternion::to_matrix to get the rotation matrix.
    Quaternion orientation;

    // The world scale of the object.
    Vector3 scale;

    /**
     * Renders this geometry using OpenGL in the local coordinate space.
     */
    virtual void render() const = 0;

    // ------------------------------------------------------------------------
    // variables and functions used for ray tracing
    // ------------------------------------------------------------------------

    /**
     * ray casting algorithm, check if a given line will hit this geometry.
     * line is already in geomtry's coordinates.
     *
     * @param[in]  line         The line object.
     * @param[in]  tMin         The minimum legal number of t.
     * @param[in]  tMax         The maximum legal number of t.
     * @param[out] t            Time the ray cost to hit this geometry.
     * @param[out] hit_vertex   A HitVertexInfor object which contains hit
     *                          vertex information.
     * @return true if given line hit this geometry, otherwise false.
     */
    virtual bool ray_casting(const Ray&      line,
                             const real_t    tMin, 
                             const real_t    tMax,
                             float&          t, 
                             HitVertexInfor& hit_vertex) = 0;

    /**
     * build the inverse transformation matrix, used for ray tracing.
     */
    void build_inverse_transformation_matrix();

    // inverse transformation matrix
    Matrix4 m_invTransformMat;  
    // inverse transformation matrix, used for ray's direction
    Matrix4 m_invTransformMatWithoutTranslation; 
    // transformation matrix
    Matrix4 m_transformatMat;
    // normal matrix, which is transpose of m_invTransformMatWithoutTranslation
    Matrix3 m_normalMatrix;

    // a tag that if this sphere has computed inverse transformation matrix
    bool m_bNoTransformationMatrix;

};


struct PointLight
{
    struct Attenuation
    {
        real_t constant;
        real_t linear;
        real_t quadratic;
    };

    PointLight();

    // The position of the light, relative to world origin.
    Vector3 position;
    // The color of the light (both diffuse and specular)
    Color3 color;
    // attenuation
    Attenuation attenuation;

    inline Color3 get_attenuation_color(float distance) 
    {
        float attenuation_factor = 1 / (attenuation.constant + 
                                        distance * attenuation.linear + 
                                        distance * distance * attenuation.quadratic);
        return color * attenuation_factor;
    }
};

/**
 * The container class for information used to render a scene composed of
 * Geometries.
 */
class Scene
{
public:

    /// the camera
    Camera camera;
    /// the background color
    Color3 background_color;
    /// the amibient light of the scene
    Color3 ambient_light;
    /// the refraction index of air
    real_t refractive_index;

    /// Creates a new empty scene.
    Scene();

    /// Destroys this scene. Invokes delete on everything in geometries.
    ~Scene();

    // accessor functions
    Geometry* const* get_geometries() const;
    size_t num_geometries() const;
    const PointLight* get_lights() const;
    size_t num_lights() const;
    Material* const* get_materials() const;
    size_t num_materials() const;
    Mesh* const* get_meshes() const;
    size_t num_meshes() const;

    /// Clears the scene, and invokes delete on everything in geometries.
    void reset();

    // functions to add things to the scene
    // all pointers are deleted by the scene upon scene deconstruction.
    void add_geometry( Geometry* g );
    void add_material( Material* m );
    void add_mesh( Mesh* m );
    void add_light( const PointLight& l );

    bool load(const char* filename);
    bool reload() { reset(); return load(m_filename.c_str()); };

private:

    typedef std::vector< PointLight > PointLightList;
    typedef std::vector< Material*  > MaterialList;
    typedef std::vector< Mesh*      > MeshList;
    typedef std::vector< Geometry*  > GeometryList;

    // list of all lights in the scene
    PointLightList point_lights;
    // all materials used by geometries
    MaterialList materials;
    // all meshes used by models
    MeshList meshes;
    // list of all geometries. deleted in dctor, so should be allocated on heap.
    GeometryList geometries;
    std::string m_filename;

private:

    // no meaningful assignment or copy
    Scene(const Scene&);
    Scene& operator=(const Scene&);

};

} /* Luc */

#endif /* _462_SCENE_SCENE_HPP_ */

