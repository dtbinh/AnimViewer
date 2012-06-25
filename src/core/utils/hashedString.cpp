#include "lucPCH.h"
#include "hashedString.h"
#include "math/mathUtils.h"

namespace Luc
{
namespace Common
{
    //////////////////////////////////////////////////////////////////////////
    // HashedString class
    //////////////////////////////////////////////////////////////////////////
    HashedString::HashedString( const char* str ) : m_string(str), m_hash(Math::ELFHash(str))
    {

    }

    HashedString::HashedString( const std::string& str ) : m_string(str), m_hash(Math::ELFHash(str.c_str()))
    {

    }

    HashedString::HashedString( const HashedString& str ) : m_string(str.m_string), m_hash(str.m_hash)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    // functions
    //////////////////////////////////////////////////////////////////////////
    bool HashedString::operator==( const HashedString& rhv ) const
    {
#ifdef DEBUG
        if (this->GetHash() != rhv.GetHash())
        {
            return false;
        }
        else
        {
            assert (this->GetString() != rhv.GetString());
            return true;
        }
#else
        return (this->GetHash() == rhv.GetHash());
#endif
    }
}
}