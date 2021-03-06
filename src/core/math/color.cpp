/**
 * @file color.cpp
 * @brief Color class
 *
 * @author Eric Butler (edbutler)
 */
#include "lucPCH.h"
#include "math/color.hpp"

namespace Luc {

const Color3 Color3::Black = Color3( 0.0, 0.0, 0.0 );
const Color3 Color3::White = Color3( 1.0, 1.0, 1.0 );
const Color3 Color3::Red   = Color3( 1.0, 0.0, 0.0 );
const Color3 Color3::Green = Color3( 0.0, 1.0, 0.0 );
const Color3 Color3::Blue  = Color3( 0.0, 0.0, 1.0 );

Color3::Color3( const unsigned char* arr )
{
    static const real_t inv = 1.0f / 255.0f;
    r = arr[0] * inv;
    g = arr[1] * inv;
    b = arr[2] * inv;
}

void Color3::to_array( unsigned char arr[4] ) const
{
    // clamp values
    Color3 tmp = clamp( *this, 0.0, 1.0 );

    // convert to ints
    arr[0] = static_cast<unsigned char>( tmp.r * 0xff );
    arr[1] = static_cast<unsigned char>( tmp.g * 0xff );
    arr[2] = static_cast<unsigned char>( tmp.b * 0xff );
    arr[3] = 0xff;
}

void Color3::to_array( float arr[DIM] ) const
{
    arr[0] = float( r );
    arr[1] = float( g );
    arr[2] = float( b );
}

Color3 clamp( const Color3& c, real_t min, real_t max )
{
    return Color3(
        clamp( c.r, min, max ),
        clamp( c.g, min, max ),
        clamp( c.b, min, max )
    );
}

std::ostream& operator<<( std::ostream& os, const Color3& c )
{
    return os << '(' << c.r << ',' << c.g << ',' << c.b << ')';
}

} 

