/**
 * @file vector.hpp
 * @brief Vector classes.
 *
 * @author Eric Butler (edbutler)
 * @author Yantong Liu
 */

#ifndef _MATH_VECTOR_HPP_
#define _MATH_VECTOR_HPP_

#include "math/math.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

namespace Luc {

/*
This file defines 3 different vector classes: 2D, 3D, and 4D.
Each class overrides most sensible operators and have functions
for other operators such as dot product, length, and normalization.

All named functions do not mutate their arguments but rather return
a new vector. So, for example, you must do
    v = normalize( v );
in order to make v a unit vector. Some operators (e.g., +=) mutate
their first argument, as with built-in numerical types.

Most operations are inline functions because it results in a noticeable
speed increase in most applications.
*/

/**
 * A 2d vector.
 */
class Vector2
{
public:

    static const size_t  DIM = 2;   ///< The number of dimensions.
    static const Vector2 Zero;      ///< The zero vector.
    static const Vector2 Ones;      ///< The vector (1,1).
    static const Vector2 UnitX;     ///< The vector (1,0).
    static const Vector2 UnitY;     ///< The vector (0,1).

    /**
     * Components of this vector.
     */
    union {
        real_t m_data[2];
        struct { real_t x, y; };
        struct { real_t u, v; };
    };

    /**
     * constructors. 
     */
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2( real_t x, real_t y ) : x( x ), y( y ) {}

    //
    // operators
    // 
    Vector2 operator+( const Vector2& rhs ) const { return Vector2( x + rhs.x, y + rhs.y ); }
    Vector2 operator-( const Vector2& rhs ) const { return Vector2( x - rhs.x, y - rhs.y ); }
    Vector2 operator*( real_t s           ) const { return Vector2( x * s, y * s ); }
    Vector2 operator/( real_t s           ) const { real_t inv = 1.0f / s; return Vector2( x * inv, y * inv ); }
    Vector2 operator-(                    ) const { return Vector2( -x, -y ); }

    Vector2& operator+=( const Vector2& rhs ) { x += rhs.x; y += rhs.y; return *this; }
    Vector2& operator-=( const Vector2& rhs ) { x -= rhs.x; y -= rhs.y; return *this; }
    Vector2& operator*=( real_t s           ) { x *= s; y *= s; return *this; }
    Vector2& operator/=( real_t s           ) { real_t inv = 1.0f / s; x *= inv; y *= inv; return *this; }


    /**
     * @remark No bounds checking.
     */
    const real_t& operator[]( size_t i ) const { assert( i >= 0 && i < DIM ); return m_data[i]; }
          real_t& operator[]( size_t i )       { assert( i >= 0 && i < DIM ); return m_data[i]; }

    bool operator==( const Vector2& rhs ) const { return x == rhs.x && y == rhs.y; }
    bool operator!=( const Vector2& rhs ) const { return !operator==( rhs ); }

    void to_array( float arr[DIM] ) const {
        arr[0] = m_data[0];
        arr[1] = m_data[1];
    }

    real_t  squared_length() const { return x*x + y*y; }
    real_t  length()         const { return sqrt(squared_length()); }
    void    normalize()            {  *this / length(); }
};

/**
 * Returns the dot product of two vectors
 */
inline real_t dot( const Vector2& lhs, const Vector2& rhs ) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

/**
 * Efficiency function: does not require square root operation.
 */
inline real_t squared_length( const Vector2& v ) {
    return v.squared_length();
}

/**
 * Returns the length of a vector.
 */
inline real_t length( const Vector2& v ) {
    return v.length();
}

/**
 * Calculate the positive distance between two vectors.
 */
inline real_t distance( const Vector2& lhs, const Vector2& rhs ) {
    return length( lhs - rhs );
}

/**
 * Efficiency function: does not require square root operation.
 */
inline real_t squared_distance( const Vector2& lhs, const Vector2& rhs ) {
    return squared_length( lhs - rhs );
}

/**
 * Returns the unit vector pointing in the same direction as this vector.
 */
inline Vector2 normalize( const Vector2& v ) {
    return v / length( v );
}

/**
 * Returns a vector whose elements are the absolute values of all the
 * elements of this vector.
 */
inline Vector2 vabs( const Vector2& v ) {
    return Vector2( fabs( v.x ), fabs( v.y ) );
}

/**
 * Returns the element-wise maximum of the two vectors.
 */
inline Vector2 vmax( const Vector2& lhs, const Vector2& rhs ) {
    return Vector2(
        std::max( lhs.x, rhs.x ),
        std::max( lhs.y, rhs.y )
    );
}

/**
 * Returns the element-wise minimum of the two vectors.
 */
inline Vector2 vmin( const Vector2& lhs, const Vector2& rhs ) {
    return Vector2(
        std::min( lhs.x, rhs.x ),
        std::min( lhs.y, rhs.y )
    );
}

inline Vector2 operator*( real_t s, const Vector2& rhs ) {
    return rhs * s;
}

/**
 * Outputs a vector text formatted as "(x,y,z)".
 */
std::ostream& operator<<( std::ostream& os, const Vector2& rhs );

/**
 * A 3d vector.
 */
class Vector3
{
public:
    static const size_t  DIM = 3;   ///< The number of dimensions.
    static const Vector3 Zero;      ///< The zero vector.
    static const Vector3 Ones;      ///< The vector (1,1,1).
    static const Vector3 UnitX;     ///< The vector (1,0,0)
    static const Vector3 UnitY;     ///< The vector (0,1,0)
    static const Vector3 UnitZ;     ///< The vector (0,0,1)

    /**
     * Components of this vector.
     */
    union {
        real_t m_data[3];
        struct { real_t x, y, z; };
    };

    /**
     * constructors
     */
    Vector3() {}
    Vector3( real_t x, real_t y, real_t z ) : x( x ), y( y ), z( z ) {}
    Vector3( const Vector2& v, real_t z )  : x( v.x ), y( v.y ), z( z ) { }
    explicit Vector3( const float arr[3] ) : x( arr[0] ), y( arr[1] ), z( arr[2] ) { }

    // operators
    Vector3 operator+( const Vector3& rhs ) const { return Vector3( x + rhs.x, y + rhs.y, z + rhs.z ); }
    Vector3 operator-( const Vector3& rhs ) const { return Vector3( x - rhs.x, y - rhs.y, z - rhs.z ); }
    Vector3 operator*( real_t s ) const { return Vector3( x * s, y * s, z * s ); }
    Vector3 operator/( real_t s ) const { real_t inv = 1.0f / s; return Vector3( x * inv, y * inv, z * inv ); }
    Vector3 operator-() const { return Vector3( -x, -y, -z ); }

    Vector3& operator+=( const Vector3& rhs ) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
    Vector3& operator-=( const Vector3& rhs ) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
    Vector3& operator*=( real_t s ) { x *= s; y *= s; z *= s; return *this; }
    Vector3& operator/=( real_t s ) { real_t inv = 1.0f / s; x *= inv; y *= inv; z *= inv; return *this; }
    
    const real_t& operator[]( size_t i ) const { assert( i >= 0 && i < DIM ); return m_data[i]; }
          real_t& operator[]( size_t i )       { assert( i >= 0 && i < DIM ); return m_data[i]; }

    bool operator==( const Vector3& rhs ) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator!=( const Vector3& rhs ) const { return !operator==( rhs ); }

    void to_array( float arr[DIM] ) const {
        arr[0] = float( x );
        arr[1] = float( y );
        arr[2] = float( z );
    }

    real_t  squared_length() const { return x*x + y*y + z*z; }
    real_t  length()         const { return sqrt(squared_length()); }
    void    normalize()            {  *this / length(); }
};

/**
 * Returns the dot product of two vectors
 */
inline real_t dot( const Vector3& lhs, const Vector3& rhs ) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

/**
 * Returns the cross product of two vectors
 */
inline Vector3 cross( const Vector3& lhs, const Vector3& rhs ) {
    return Vector3(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    );
}

/**
 * Efficiency function: does not require square root operation.
 */
inline real_t squared_length( const Vector3& v ) {
    return v.squared_length();
}

/**
 * Returns the length of a vector.
 */
inline real_t length( const Vector3& v ) {
    return v.length();
}

/**
 * Calculate the positive distance between two vectors.
 */
inline real_t distance( const Vector3& lhs, const Vector3& rhs ) {
    return length( lhs - rhs );
}

/**
 * Efficiency function: does not require square root operation.
 */
inline real_t squared_distance( const Vector3& lhs, const Vector3& rhs ) {
    return squared_length( lhs - rhs );
}

/**
 * Returns the unit vector pointing in the same direction as this vector.
 */
inline Vector3 normalize( const Vector3& v ) {
    return v / length( v );
}

/**
 * Returns a vector whose elements are the absolute values of all the
 * elements of this vector.
 */
inline Vector3 vabs( const Vector3& v ) {
    return Vector3( fabs( v.x ), fabs( v.y ), fabs( v.z ) );
}

/**
 * Returns the element-wise maximum of the two vectors.
 */
inline Vector3 vmax( const Vector3& lhs, const Vector3& rhs ) {
    return Vector3(
        std::max( lhs.x, rhs.x ),
        std::max( lhs.y, rhs.y ),
        std::max( lhs.z, rhs.z )
    );
}

/**
 * Returns the element-wise minimum of the two vectors.
 */
inline Vector3 vmin( const Vector3& lhs, const Vector3& rhs ) {
    return Vector3(
        std::min( lhs.x, rhs.x ),
        std::min( lhs.y, rhs.y ),
        std::min( lhs.z, rhs.z )
    );
}

inline Vector3 operator*( real_t s, const Vector3& rhs ) {
    return rhs * s;
}

/**
 * Outputs a vector text formatted as "(x,y,z)".
 */
std::ostream& operator<<( std::ostream& os, const Vector3& rhs );

/**
 * A 4d vector.
 */
class Vector4
{
public:
    static const size_t  DIM = 4;   ///< The number of dimensions.
    static const Vector4 Zero;      ///< The zero vector.
    static const Vector4 Ones;      ///< The vector (1,1,1,1).
    static const Vector4 UnitX;     ///< The vector (1,0,0,0)
    static const Vector4 UnitY;     ///< The vector (0,1,0,0)
    static const Vector4 UnitZ;     ///< The vector (0,0,1,0)
    static const Vector4 UnitW;     ///< The vector (0,0,0,1)

    /**
     * Components of this vector.
     */
    union {
        real_t m_data[4];
        struct { real_t x, y, z, w; };
    };
    

    /**
     * Default constructor. Leaves values uninitialized.
     */
    Vector4() {}
    Vector4( real_t x, real_t y, real_t z, real_t w ) : x( x ), y( y ), z( z ), w( w ) {}
    Vector4( const Vector3& v, real_t w ) : x( v.x ), y( v.y ), z( v.z ), w( w ) {}

    // also uses default copy and assignment

    Vector4 operator+( const Vector4& rhs ) const { return Vector4( x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w ); }
    Vector4 operator-( const Vector4& rhs ) const { return Vector4( x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w ); }
    Vector4 operator*( real_t s ) const { return Vector4( x * s, y * s, z * s, w * s ); }
    Vector4 operator/( real_t s ) const { real_t inv = 1.0f / s; return Vector4( x * inv, y * inv, z * inv, w * inv ); }
    Vector4 operator-() const { return Vector4( -x, -y, -z, -w ); }

    Vector4& operator+=( const Vector4& rhs ) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
    Vector4& operator-=( const Vector4& rhs ) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
    Vector4& operator*=( real_t s ) { x *= s; y *= s; z *= s; w *= s; return *this; }
    Vector4& operator/=( real_t s ) { real_t inv = 1.0f / s; x *= inv; y *= inv; z *= inv; w *= inv; return *this; }

    const real_t& operator[]( size_t i ) const { assert( i >= 0 && i < DIM ); return m_data[i]; }
          real_t& operator[]( size_t i )       { assert( i >= 0 && i < DIM ); return m_data[i]; }

    bool operator==( const Vector4& rhs ) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
    bool operator!=( const Vector4& rhs ) const { return !operator==( rhs ); }

    /**
     * Returns the first three components, ignoring the fourth
     */
    Vector3 xyz() const { return Vector3( x, y, z ); }

    void to_array( float arr[DIM] ) const {
        arr[0] = float( x );
        arr[1] = float( y );
        arr[2] = float( z );
        arr[3] = float( w );
    }

    real_t  squared_length() const { return x*x + y*y + z*z + w*w; }
    real_t  length()         const { return sqrt(squared_length()); }
    void    normalize()            {  *this / length(); }
};

/**
 * Returns the 3d vector corresponding to this 4d vector.
 * @remark If w==0, returns (x,y,z).
 */
inline Vector3 project( const Vector4& v ) {
    real_t winv = v.w == 0.0 ? 1.0f : 1.0f / v.w;
    return Vector3( v.x * winv, v.y * winv, v.z * winv );
}

/**
 * Returns the dot product of two vectors
 */
inline real_t dot( const Vector4& lhs, const Vector4& rhs ) {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

/**
 * Efficiency function: does not require square root operation.
 */
inline real_t squared_length( const Vector4& v ) {
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

/**
 * Returns the length of a vector.
 */
inline real_t length( const Vector4& v ) {
    return sqrt( squared_length( v ) );
}

/**
 * Calculate the positive distance between two vectors.
 */
inline real_t distance( const Vector4& lhs, const Vector4& rhs ) {
    return length( lhs - rhs );
}

/**
 * Efficiency function: does not require square root operation.
 */
inline real_t squared_distance( const Vector4& lhs, const Vector4& rhs ) {
    return squared_length( lhs - rhs );
}

/**
 * Returns the unit vector pointing in the same direction as this vector.
 */
inline Vector4 normalize( const Vector4& v ) {
    return v / length( v );
}

/**
 * Returns a vector whose elements are the absolute values of all the
 * elements of this vector.
 */
inline Vector4 vabs( const Vector4& v ) {
    return Vector4( fabs( v.x ), fabs( v.y ), fabs( v.z ), fabs( v.w ) );
}

/**
 * Returns the element-wise maximum of the two vectors.
 */
inline Vector4 vmax( const Vector4& lhs, const Vector4& rhs ) {
    return Vector4(
        std::max( lhs.x, rhs.x ),
        std::max( lhs.y, rhs.y ),
        std::max( lhs.z, rhs.z ),
        std::max( lhs.w, rhs.w )
    );
}

/**
 * Returns the element-wise minimum of the two vectors.
 */
inline Vector4 vmin( const Vector4& lhs, const Vector4& rhs ) {
    return Vector4(
        std::min( lhs.x, rhs.x ),
        std::min( lhs.y, rhs.y ),
        std::min( lhs.z, rhs.z ),
        std::min( lhs.w, rhs.w )
    );
}

inline Vector4 operator*( real_t s, const Vector4& rhs ) {
    return rhs * s;
}

/**
 * Outputs a vector text formatted as "(x,y,z)".
 */
std::ostream& operator<<( std::ostream& os, const Vector4& rhs );

} 

#endif /* _MATH_VECTOR_HPP_ */

