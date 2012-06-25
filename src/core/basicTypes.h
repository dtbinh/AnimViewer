#ifndef CORE_BASIC_TYPES_H
#define CORE_BASIC_TYPES_H

namespace Luc
{
#ifdef _MSC_VER
    typedef signed   __int8     int8_t;
    typedef signed   __int16    int16_t;
    typedef signed   __int32    int32_t;
    typedef signed   __int64    int64_t;

    typedef unsigned __int8     uint8_t;
    typedef unsigned __int16    uint16_t;
    typedef unsigned __int32    uint32_t;
    typedef unsigned __int64    uint64_t;

    const uint32_t MAX_UINT_8  = 0xFF;
    const uint32_t MAX_UINT_16 = 0xFFFF;
    const uint32_t MAX_UINT_32 = 0xFFFFFFFF;
#endif

}

#endif // CORE_BASIC_TYPES_H
