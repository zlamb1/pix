#ifndef PIX_CLOCK_H
#define PIX_CLOCK_H 1

#ifdef PIX_PLATFORM_LINUX

#include <unistd.h>

#define PIX_CLOCK_START() \
    struct timespec start, end; \
    clock_gettime(CLOCK_MONOTONIC, &start)

#define PIX_CLOCK_END() clock_gettime(CLOCK_MONOTONIC, &end)

#define PIX_CLOCK_PRINT() \
    do { \
        struct timespec delta; \
        delta.tv_nsec = end.tv_nsec - start.tv_nsec; \
        delta.tv_sec = end.tv_sec - start.tv_sec; \
        printf("%fms\n", delta.tv_sec * 1000.0 + delta.tv_nsec / 1000000.0); \
    } while (0)

#endif

#endif
