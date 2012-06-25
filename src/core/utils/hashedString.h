#ifndef CORE_UTILS_HASHEDSTRING_H
#define CORE_UTILS_HASHEDSTRING_H

#include <string>
namespace Luc
{
namespace Common
{

    class HashedString
    {
    public:
        HashedString(const char* str);
        HashedString(const std::string& str);
        HashedString(const HashedString& str);
        ~HashedString() {};

        unsigned long       GetHash()   const   {   return m_hash;      }
        const std::string&  GetString() const   {   return m_string;    }

        bool operator==(const HashedString& rhv) const ;
        bool operator< (const HashedString& rhv) const {return GetHash() < rhv.GetHash(); };
        bool operator> (const HashedString& rhv) const {return GetHash() > rhv.GetHash(); };

    private:
        unsigned long   m_hash;
        std::string     m_string;
    };

    //bool operator==(const HashedString& lhv, const HashedString& rhv);
    //inline bool operator< (const HashedString& lhv, const HashedString& rhv)   {return lhv.GetHash() < rhv.GetHash(); };
    //inline bool operator> (const HashedString& lhv, const HashedString& rhv)   {return lhv.GetHash() > rhv.GetHash(); };
}
}

#endif // CORE_UTILS_HASHEDSTRING_H
