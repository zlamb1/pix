#ifndef PIX_PACK_H
#define PIX_PACK_H 1

#ifdef __GNUC__
#define PIX_PACK(__STRUCT__) __STRUCT__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PIX_PACK( __STRUCT__ ) __pragma( pack(push, 1) ) __STRUCT__ __pragma( pack(pop))
#endif

#endif
