#ifndef HEADER_COMMON_COMMONTYPE
#define HEADER_COMMON_COMMONTYPE

#include <string>

namespace hudp {

#define CHECK_RET(ret) do{if(!ret) return false;}while(0);

typedef std::string HudpHandle;
static const uint16_t __max_id = (uint16_t)0xFFFF;

#if defined(__GNUC__) || defined(__GCCXML__) || defined(__SNC__) || defined(__S3E__)

    typedef unsigned char             uint8_t;
    typedef unsigned short            uint16_t;
    typedef unsigned int              uint32_t;
    typedef signed char               int8_t;
    typedef signed short              int16_t;
    typedef int                       int32_t;
    //typedef unsigned long long int  uint64_t;
    //typedef signed long long        int64_t;

#elif !defined(_STDINT_H) && !defined(_SN_STDINT_H) && !defined(_SYS_STDINT_H_) && !defined(_STDINT) && !defined(_MACHTYPES_H_) && !defined(_STDINT_H_)
    typedef unsigned char             uint8_t;
    typedef unsigned short            uint16_t;
    typedef unsigned __int32          uint32_t;
    typedef signed char               int8_t;
    typedef signed short              int16_t;
    typedef __int32                   int32_t;
#if defined(_MSC_VER) && _MSC_VER < 1300
    typedef unsigned __int64          uint64_t;
    typedef signed __int64            int64_t;
#else
    typedef unsigned long long int    uint64_t;
    typedef signed long long          int64_t;
#endif
#endif
}

#endif