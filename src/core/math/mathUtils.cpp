#include "lucPCH.h"
#include "math/mathUtils.h"
#include <ctype.h>
#include <string.h>

namespace Luc
{
namespace Math
{
    //ELF hash
    //
    //The ELF hash function has been around for a while, and it is believed to be one of the better algorithms
    //out there. In my experience, this is true, though ELF hash does not perform sufficiently better than most
    //of the other algorithms presented in this tutorial to justify its slightly more complicated implementation.
    //It should be on your list of first functions to test in a new lookup implementation:
    uint32_t ELFHash(const char *pName)
    {
        uint32_t hash(0);
        uint32_t temp(0);
        for(const char* pStr(pName); *pStr; pStr++)
        {
            hash = (hash << 4) + tolower(*pStr);
            if (0 != (temp = hash & 0xf0000000)) 
            {
                hash = hash ^ (temp >> 24);
                hash = hash ^ temp;
            }
        }
        return hash;
    }
    //FNV hash
    //The FNV hash, short for Fowler/Noll/Vo in honor of the creators, 
    //is a very powerful algorithm that, not surprisingly, follows the 
    //same lines as Bernstein's modified hash with carefully chosen constants.
    //This algorithm has been used in many applications with wonderful results,
    // and for its simplicity, the FNV hash should be one of the first hashes 
    //tried in an application. It is also recommended that the FNV website be 
    //visited for useful descriptions of how to modify the algorithm for various uses.
    uint32_t FNVHash (const char *pName) 
    { 
        uint32_t hash((NULL != pName) ? 2166136261 : 0); 
        for(const char* pStr(pName); *pStr; ++pStr)
        {
            hash = ( hash * 16777619 ) ^ tolower(*pStr);
        }

        return hash; 
    }


    //Additive hash
    uint32_t ADDHash( const char *pName)
    {
        uint32_t hash(0);
        for (const char* pStr(pName); *pStr; ++pStr)
        {
            hash += (*pStr);
        }

        return hash;
    }



    //XOR hash
    uint32_t XORHash( const char *pName)
    {
        uint32_t hash(0);
        for (const char* pStr(pName); *pStr; ++pStr)
        {
            hash ^= (*pStr);
        }

        return hash;
    }


    //Rotating hash
    uint32_t ROTHash( const char *pName)
    {
        uint32_t hash(0);
        for (const char* pStr(pName); *pStr; ++pStr)
        {
            hash = ( hash << 4 ) ^ ( hash >> 28 ) ^ (*pStr);
        }

        return hash;
    }

    //Bernstein hash
    //
    //Dan Bernstein created this algorithm and posted it in a newsgroup. 
    //It is known by many as the Chris Torek hash because Chris went a long 
    //way toward popularizing it. Since then it has been used successfully by 
    //many, but despite that the algorithm itself is not very sound when it comes 
    //to avalanche and permutation of the internal state. It has proven very good 
    //for small character keys, where it can outperform algorithms that result in 
    //a more random distribution:
    //Bernstein's hash should be used with caution. It performs very well in practice, 
    //for no apparently known reasons (much like how the constant 33 does better than 
    //	more logical constants for no apparent reason), 
    // but in theory it is not up to snuff. Always 
    //	 test this function with sample data for every 
    //	 application to ensure that it does not encounter 
    //	 a degenerate case and cause excessive collisions.
    //
    uint32_t DJBHash( const char *pName)
    {
        uint32_t hash(0);
        for (const char* pStr(pName); *pStr; ++pStr)
        {
            hash = 33*hash + (*pStr);
        }

        return hash;
    }

    //Shift-Add-XOR hash
    //
    //The shift-add-XOR hash was designed as a string hashing function, 
    //but because it is so effective, it works for any data as well with similar efficiency. 
    //The algorithm is surprisingly similar to the rotating hash except a different choice 
    //of constants for the rotation is used, and addition is a preferred operation for mixing.
    // All in all, this is a surprisingly powerful and flexible hash. 
    //Like many effective hashes, it will fail tests for avalanche, 
    //but that does not seem to affect its performance in practice.
    //
    uint32_t SAXHash( const char *pName)
    {
        uint32_t hash(0);
        for (const char* pStr(pName); *pStr; ++pStr)
        {
            hash ^= (hash << 5) + (hash >> 2) + (*pStr);
        }

        return hash;
    }

    //One-at-a-Time hash
    //
    //Bob Jenkins is a well known authority on designing hash functions for table lookup. 
    //In fact, one of his hashes is considered state of the art for lookup, which we 
    //will see shortly. A considerably simpler algorithm of his design is the One-at-a-Time hash:
    //
    uint32_t OATHash( const char *pName)
    {
        uint32_t hash(0);
        for (const char* pStr(pName); *pStr; ++pStr)
        {
            hash += (*pStr); 
            hash += ( hash << 10 ); 
            hash ^= ( hash >> 6 ); 
        }
        hash += ( hash << 3 );
        hash ^= ( hash >> 11 ); 
        hash += ( hash << 15 ); 
        return hash;
    }
}
}