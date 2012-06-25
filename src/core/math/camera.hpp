/**
 * @file camera.hpp
 * @brief Camera class
 *
 * @author Eric Butler (edbutler)
 */

#ifndef _LUC_MATH_CAMERA_HPP_
#define _LUC_MATH_CAMERA_HPP_

#include "math/vector.hpp"
#include "math/quaternion.hpp"

namespace Luc {

/**
 * Stores position data of the camera.
 */
class Camera
{
public:
    Camera() : 
        mPosition( Vector3::Zero ),
        mOrientation( Quaternion::Identity ),
        mFov( PI / 4.0f ),
        mAspectRatio( 1 ),
        mNearClip( .1f ),
        mFarClip( 10 )
    { }

    // accessor functions

    const Vector3& GetPosition() const { return mPosition; }    //< Returns the world position.
    Vector3 GetDirection() const {return mOrientation * -Vector3::UnitZ;}
    Vector3 GetUp() const {return mOrientation * Vector3::UnitY;}

    // Returns the field of view in radians of the y-axis.
    real_t GetFovRadians() const { return mFov; }
    // Returns the field of view in degrees of the y-axis.
    real_t GetFovDegrees() const { return mFov * 180.0f / PI; }
    // Returns the aspect ratio (width/height).
    real_t GetAspectRatio() const {return mAspectRatio;}
    // Returns the distance from the camera to the near clipping plane.
    real_t GetNearClip()   const {return mNearClip;}
    // Returns the distance from the camera to the far clipping plane.
    real_t GetFarClip()    const {return mFarClip;}

    void SetPosition(const Vector3& newPosition) {mPosition = newPosition;}
    void SetFOV(real_t fov)             { mFov = fov;           }
    void SetAspectRatio(real_t ratio)   { mAspectRatio = ratio; }
    void SetNearClip(real_t nearClip)   { mNearClip = nearClip; }
    void SetFarClip(real_t farClip)     { mFarClip = farClip;   }
    void SetOrientation(const Quaternion& orientation) {mOrientation = orientation;}

    // translates camera position by v
    void Translate( const Vector3& v ) { mPosition += mOrientation * v; }
    // rotates camera about its X axis
    void Pitch( real_t radians ) {Rotate( mOrientation * Vector3::UnitX, radians );}
    // rotates camera about its Z axis
    void Roll( real_t radians ) {Rotate( mOrientation * Vector3::UnitZ, radians );}
    // rotates camera about its Y axis
    void Yaw( real_t radians )  {Rotate( mOrientation * Vector3::UnitY, radians );}

private:
    // rotates camera about the given axis
    void Rotate( const Vector3& axis, real_t radians ) {mOrientation = normalize( Quaternion( axis, radians ) * mOrientation );}

private:
    // The orientation of the camera, relative to a default direction
    // of negative z axis and default up vector of y axis.
    Quaternion mOrientation;
    Vector3 mPosition;  // The world position of the camera.
    real_t mAspectRatio;// The aspect ratio (width/height).
    real_t mFov;        // Field of view of y-axis, in radians.
    real_t mNearClip;
    real_t mFarClip;
};

} 

#endif /* _LUC_MATH_CAMERA_HPP_ */

