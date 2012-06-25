#pragma once
#ifndef RAY_CASTING_H
#define RAY_CASTING_H



#include "math/ray.hpp"
#include "math/vector.hpp"
#include "math/color.hpp"
#include "scene/material.hpp"

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
                          float& t,          float& beta,       float& gamma);

/*
 * Barycentric interpolation functions.
 */
inline float barycentric_interpolation( float f0, float f1, float f2, 
                            float beta, float gamma )
{
    return (1-beta-gamma) * f0 + beta * f1 + gamma * f2;
}

inline Vector3 barycentric_interpolation( Vector3 v0, Vector3 v1, Vector3 v2, 
                              float beta, float gamma )
{
    return Vector3( barycentric_interpolation(v0.x, v1.x, v2.x, beta, gamma),
                    barycentric_interpolation(v0.y, v1.y, v2.y, beta, gamma),
                    barycentric_interpolation(v0.z, v1.z, v2.z, beta, gamma) );
}

inline Color3 barycentric_interpolation( Color3 c0, Color3 c1, Color3 c2, 
                             float beta, float gamma )
{
    return Color3( barycentric_interpolation(c0.r, c1.r, c2.r, beta, gamma),
                   barycentric_interpolation(c0.g, c1.g, c2.g, beta, gamma),
                   barycentric_interpolation(c0.b, c1.b, c2.b, beta, gamma) );
}

/*
 * Linear interpolation function.
 */
inline float linear_interpolation(float num1, float num2, float factor)
{
    return num1 * factor + num2 * (1-factor);
}

/*
 * Bilinear interpolation functions.
 */
inline float bilinear_interpolation(float lt, float lb, float rt, float rb,
                                    float horizontal_factor, float vertical_factor)
{
    float top = linear_interpolation(lt, rt, horizontal_factor);
    float bottom = linear_interpolation(lb, rb, horizontal_factor);
    return linear_interpolation(top, bottom, vertical_factor);
}

inline Color3 bilinear_interpolation(Color3 lrc, Color3 lbc, Color3 rtc, Color3 rbc,
                                     float horizontal_factor, float vertical_factor)
{
    Color3 color;
    color.r = bilinear_interpolation(lrc.r, lbc.r, rtc.r, rbc.r, horizontal_factor, vertical_factor);
    color.g = bilinear_interpolation(lrc.g, lbc.g, rtc.g, rbc.g, horizontal_factor, vertical_factor);
    color.b = bilinear_interpolation(lrc.b, lbc.b, rtc.b, rbc.b, horizontal_factor, vertical_factor);
    return color;
}

/*
 * Texture color interpolation function.
 * @param tex_coord Texture coordinates.
 * @param material  Material pointer, which should get from the geometry which 
 *                  we need interpolate texture color.
 */
Color3 interpolate_texture_color(Vector2 tex_coord, const Material* material);


} // namespace Luc

#endif // RAY_CASTING_H
