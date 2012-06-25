/**
 * @file scene.cpp
 * @brief Function definitions for scenes.
 *
 * @author Eric Butler (edbutler)
 * @author Kristin Siu (kasiu)
 */
#include "lucPCH.h"
#include "scene/scene.hpp"
#include "scene/scene_loader.hpp"

namespace Luc {


Geometry::Geometry():
    position( Vector3::Zero ),
    orientation( Quaternion::Identity ),
    scale( Vector3::Ones )
{
}

Geometry::~Geometry() { }

/**
 * build the inverse transformation matrix, used for ray tracing.
 */
void Geometry::build_inverse_transformation_matrix() 
{
    make_inverse_transformation_matrix(&m_invTransformMat, position,
        orientation,  scale);
    make_inverse_transformation_matrix(&m_invTransformMatWithoutTranslation,
        Vector3(0, 0, 0), orientation, scale);
    make_transformation_matrix(&m_transformatMat, position, orientation, scale);

    Matrix3 simple_inverse_transformation_matrix;
    simple_inverse_transformation_matrix._m[0][0] = m_invTransformMatWithoutTranslation._m[0][0];
    simple_inverse_transformation_matrix._m[0][1] = m_invTransformMatWithoutTranslation._m[0][1];
    simple_inverse_transformation_matrix._m[0][2] = m_invTransformMatWithoutTranslation._m[0][2];
    simple_inverse_transformation_matrix._m[1][0] = m_invTransformMatWithoutTranslation._m[1][0];
    simple_inverse_transformation_matrix._m[1][1] = m_invTransformMatWithoutTranslation._m[1][1];
    simple_inverse_transformation_matrix._m[1][2] = m_invTransformMatWithoutTranslation._m[1][2];
    simple_inverse_transformation_matrix._m[2][0] = m_invTransformMatWithoutTranslation._m[2][0];
    simple_inverse_transformation_matrix._m[2][1] = m_invTransformMatWithoutTranslation._m[2][1];
    simple_inverse_transformation_matrix._m[2][2] = m_invTransformMatWithoutTranslation._m[2][2];
    transpose(&m_normalMatrix, simple_inverse_transformation_matrix);
}



PointLight::PointLight():
    position( Vector3::Zero ),
    color( Color3::White )
{
    attenuation.constant = 1;
    attenuation.linear = 0;
    attenuation.quadratic = 0;
}


Scene::Scene()
{
    reset();
}

Scene::~Scene()
{
    reset();
}

Geometry* const* Scene::get_geometries() const
{
    return geometries.empty() ? NULL : &geometries[0];
}

size_t Scene::num_geometries() const
{
    return geometries.size();
}

const PointLight* Scene::get_lights() const
{
    return point_lights.empty() ? NULL : &point_lights[0];
}

size_t Scene::num_lights() const
{
    return point_lights.size();
}

Material* const* Scene::get_materials() const
{
    return materials.empty() ? NULL : &materials[0];
}

size_t Scene::num_materials() const
{
    return materials.size();
}

Mesh* const* Scene::get_meshes() const
{
    return meshes.empty() ? NULL : &meshes[0];
}

size_t Scene::num_meshes() const
{
    return meshes.size();
}

void Scene::reset()
{
    for ( GeometryList::iterator i = geometries.begin(); i != geometries.end(); ++i ) {
        delete *i;
    }
    for ( MaterialList::iterator i = materials.begin(); i != materials.end(); ++i ) {
        delete *i;
    }
    for ( MeshList::iterator i = meshes.begin(); i != meshes.end(); ++i ) {
        delete *i;
    }

    geometries.clear();
    materials.clear();
    meshes.clear();
    point_lights.clear();

    camera = Camera();

    background_color = Color3::Black;
    ambient_light = Color3::Black;
    refractive_index = 1.0;
}

void Scene::add_geometry( Geometry* g )
{
    geometries.push_back( g );
}

void Scene::add_material( Material* m )
{
    materials.push_back( m );
}

void Scene::add_mesh( Mesh* m )
{
    meshes.push_back( m );
}

void Scene::add_light( const PointLight& l )
{
    point_lights.push_back( l );
}

bool Scene::load( const char* filename )
{
    m_filename = filename;
    if ( !Luc::load_scene( this, filename ) ) {
        std::cout << "Error loading scene " << filename << ". Aborting.\n";
        return false;
    }
    return true;
}


} /* Luc */

