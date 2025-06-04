#ifndef PIX_ARRAY_H 
#define PIX_ARRAY_H 1

#include <stdlib.h>

#define PIX_ARRAY_MAYBE_EXPAND(BUF, ELSIZE, SIZE, CAP) \
    do { \
        size_t oldCap = (CAP); \
        if ( (CAP) == 0 ) \
            (CAP) = 1; \
        while ( (SIZE) >= (CAP) ) \
            (CAP) *= 2; \
        if (oldCap != (CAP)) { \
            void *p = realloc((BUF), (ELSIZE) * (CAP)); \
            if (p == NULL) goto pxAllocFail; \
            (BUF) = p; \
        } \
    } while (0)

#define PIX_ARRAY_APPEND(ELEMENT, BUF, ELSIZE, SIZE, CAP) \
    do { \
        PIX_ARRAY_MAYBE_EXPAND((BUF), (ELSIZE), (SIZE), (CAP)); \
        (BUF)[(SIZE)++] = (ELEMENT); \
    } while (0)

#endif
