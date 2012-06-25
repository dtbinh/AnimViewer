#include "lucPCH.h"
#include "ray.hpp"

#include <assert.h>
namespace Luc {


Ray::Ray( const Vector3& pnt, const Vector3& dir ) 
{
    assert( 0!=dir.x || 0!=dir.y || 0!=dir.z );
    m_pnt = pnt;
    m_dir = dir;
}

Ray::Ray( const Ray& line )
{
    assert( 0!=line.m_dir.x || 0!=line.m_dir.y || 0!=line.m_dir.z );
    m_pnt = line.m_pnt;
    m_dir = line.m_dir;
}

}
