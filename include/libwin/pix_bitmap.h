#ifndef PIX_BITMAP_H
#define PIX_BITMAP_H 1

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <immintrin.h>
#include <xmmintrin.h>

#include "pix_assert.h"

struct 
pxBitmap 
{
    unsigned char *data; 
    unsigned width, height, pitch;

    /* format */
    unsigned bpp;
    unsigned bytesPerPixel;

    unsigned mask[3];
    unsigned shift[3];
};

/**
 * color format:
 * 0x00BBGGRR
 */

static inline unsigned 
pxGetPixel(struct pxBitmap *bitmap, int x, int y)
{
    unsigned bufcolor;
    PIX_ASSERT(bitmap != NULL);

    bufcolor = (unsigned) bitmap->data[y * bitmap->pitch + x * bitmap->bytesPerPixel];
    return ((bufcolor & bitmap->mask[0]) >> bitmap->shift[0])        |
           (((bufcolor & bitmap->mask[1]) >> bitmap->shift[1]) << 8) |
           (((bufcolor & bitmap->mask[2]) >> bitmap->shift[2]) << 16);
}

static inline unsigned 
pxGetPixelBounded(struct pxBitmap *bitmap, int x, int y)
{
    PIX_ASSERT(bitmap != NULL);
    PIX_ASSERT((x | y) < 0 || x >= (long long) bitmap->width || y >= (long long) bitmap->height);
    return pxGetPixel(bitmap, x, y);
}

static inline void 
pxSetPixel(struct pxBitmap *bitmap, int x, int y, unsigned color)
{
    unsigned char *comp; 

    PIX_ASSERT(bitmap != NULL);
    PIX_ASSERT((x | y) >= 0 || x < (long long) bitmap->width || y < (long long) bitmap->height)

    comp = bitmap->data + y * bitmap->pitch + x * bitmap->bytesPerPixel;
    comp[bitmap->shift[0] >> 3] = color; 
    comp[bitmap->shift[1] >> 3] = color >> 8;
    comp[bitmap->shift[2] >> 3] = color >> 16;
}

static inline void 
pxSetPixelBounded(struct pxBitmap *bitmap, int x, int y, unsigned color)
{
    PIX_ASSERT(bitmap != NULL);
    if ((x | y) < 0 || x >= (long long) bitmap->width || y >= (long long) bitmap->height)
        return;
    pxSetPixel(bitmap, x, y, color); 
}

static inline void *
pxMemset32(void *p, int v, size_t n)
{
    unsigned cnt = n >> 2; 
    int *ip = (int *) p;
    __m128i vec = _mm_set1_epi32(v); 
    n &= 3;
    while (cnt--) {
        _mm_storeu_si128((__m128i *) ip, vec);
        ip += 4; 
    }
    while (n--)
        *ip++ = v; 
    return p;
}

static inline void
pxClearColor(struct pxBitmap *bitmap, unsigned color)
{
    unsigned *data, fcolor, stride;

    PIX_ASSERT(bitmap != NULL)
    
    data = (void *) bitmap->data;
    fcolor = ((color & 0xFF)           << bitmap->shift[0]) |
             ((color & 0xFF00)   >>  8 << bitmap->shift[1]) |
             ((color & 0xFF0000) >> 16 << bitmap->shift[2]); 
    stride = bitmap->pitch >> 2; 

    for (unsigned y = 0; y < bitmap->height; y++) {
        pxMemset32(data, fcolor, bitmap->width);
        data += stride; 
    }
}

static inline void
pxDrawLine(struct pxBitmap *bitmap, int x0, int y0, int x1, int y1, unsigned color)
{
    float x, y, dx = x1 - x0, dy = y1 - y0;
    int steps = fabsf(dx) > fabsf(dy) ? fabsf(dx) : fabsf(dy);
    
    PIX_ASSERT(bitmap != NULL); 

    if (!steps) {
        pxSetPixelBounded(bitmap, x0, y0, color);
        return; 
    }

    dx /= steps;
    dy /= steps;

    x = x0;
    y = y0;

    while (steps-- >= 0) {
        pxSetPixelBounded(bitmap, (int) (x + 0.5F), (int) (y + 0.5F), color);
        x += dx;
        y += dy;
    }
}

#endif
