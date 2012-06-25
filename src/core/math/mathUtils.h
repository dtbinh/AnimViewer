#ifndef CORE_MATH_MATHUTILS_H
#define CORE_MATH_MATHUTILS_H


namespace Luc
{
namespace Math
{
uint32_t ELFHash(const char *pName);

inline uint32_t HashString(const char *pName)  { return ELFHash(pName); };
}
}

#endif // CORE_MATH_MATHUTILS_H
