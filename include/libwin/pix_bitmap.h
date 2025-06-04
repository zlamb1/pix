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

#include "libbase/pix_assert.h"

#include "pix_color.h"
#include "pix_vec2.h"

typedef struct 
pxBitmap 
{
    unsigned char *data; 
    unsigned width, height, pitch;

    /* format */
    unsigned bpp;
    unsigned bytesPerPixel;

    unsigned mask[3];
    unsigned shift[3];
} pxBitmap;

/**
 * color format:
 * 0x00BBGGRR
 */

static inline unsigned 
pxGetPixel(pxBitmap *bitmap, int x, int y)
{
    unsigned bufcolor;
    PIX_ASSERT(bitmap != NULL);
    PIX_ASSERT((x | y) >= 0 && x < (long long) bitmap->width && y < (long long) bitmap->height);
    bufcolor = (unsigned) bitmap->data[y * bitmap->pitch + x * bitmap->bytesPerPixel];
    return ((bufcolor & bitmap->mask[0]) >> bitmap->shift[0])        |
           (((bufcolor & bitmap->mask[1]) >> bitmap->shift[1]) << 8) |
           (((bufcolor & bitmap->mask[2]) >> bitmap->shift[2]) << 16);
}

static inline void 
pxSetPixel(pxBitmap *bitmap, int x, int y, unsigned color)
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
pxSetPixelSafe(pxBitmap *bitmap, int x, int y, unsigned color)
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
    /* check pointer alignment */
    if ( (((uintptr_t) ip ) & 15) == 0 ) {
        while (cnt--) {
            _mm_store_si128((__m128i *) __builtin_assume_aligned(ip, 16), vec);
            ip += 4; 
        }
    } else {
        while (cnt--) {
            /* unaligned casts are defined for vector intrinsics */
            _mm_storeu_si128((__m128i *) (void *) ip, vec);
            ip += 4; 
        }
    }
    while (n--)
        *ip++ = v; 
    return p;
}

static inline void
pxClearColor(pxBitmap *bitmap, unsigned color)
{
    unsigned *data, fcolor, stride;

    PIX_ASSERT(bitmap != NULL)
    
    data = (unsigned *) __builtin_assume_aligned(bitmap->data, 4);
    fcolor = ((color & 0xFF)           << bitmap->shift[0]) |
             ((color & 0xFF00)   >>  8 << bitmap->shift[1]) |
             ((color & 0xFF0000) >> 16 << bitmap->shift[2]); 
    stride = bitmap->pitch >> 2; 

    if (bitmap->width == stride)
        pxMemset32(data, fcolor, bitmap->width * bitmap->height); 
    else {
        for (unsigned y = 0; y < bitmap->height; y++) {
            pxMemset32(data, fcolor, bitmap->width);
            data += stride; 
        }
    }
}

static inline void 
pxClearHLine(pxBitmap *bitmap, unsigned y, unsigned x0, unsigned x1, unsigned color)
{
    unsigned *data, fcolor;

    PIX_ASSERT(bitmap != NULL);
    PIX_ASSERT(x0 <= x1);
    PIX_ASSERT(x1 < bitmap->width); 
    PIX_ASSERT(y < bitmap->height);

    data = (unsigned *) __builtin_assume_aligned(bitmap->data, 4);
    data += (bitmap->pitch >> 2) * y + x0; 
    fcolor = ((color & 0xFF)           << bitmap->shift[0]) |
             ((color & 0xFF00)   >>  8 << bitmap->shift[1]) |
             ((color & 0xFF0000) >> 16 << bitmap->shift[2]); 

    pxMemset32(data, fcolor, x1 - x0); 
}

static inline void
pxDrawLine(pxBitmap *bitmap, int x0, int y0, int x1, int y1, unsigned color)
{
    float x, y, dx = x1 - x0, dy = y1 - y0;
    int steps = fabsf(dx) > fabsf(dy) ? fabsf(dx) : fabsf(dy);
    
    PIX_ASSERT(bitmap != NULL); 

    if (!steps) {
        pxSetPixelSafe(bitmap, x0, y0, color);
        return; 
    }

    dx /= steps;
    dy /= steps;

    x = x0;
    y = y0;

    while (steps-- >= 0) {
        pxSetPixelSafe(bitmap, (int) (x + 0.5F), (int) (y + 0.5F), color);
        x += dx;
        y += dy;
    }
}

static inline double 
pxSignedArea(pxVec2 a, pxVec2 b, pxVec2 c)
{
    return 0.5 * (a.x * (b.y - c.y) +
           b.x * (c.y - a.y) +
           c.x * (a.y - b.y));
}

static inline void 
pxDrawTriangle(pxBitmap *bitmap, pxVec2 a, pxVec2 b, pxVec2 c, pxColorRgba colors[3])
{
    pxVec2 bbBoxMin = pxMinVec2(pxMinVec2(a, b), c), 
           bbBoxMax = pxMaxVec2(pxMaxVec2(a, b), c),
           minClip  = { .x = 0, .y = 0 },
           maxClip; 
    double signedArea = pxSignedArea(a, b, c); 
    PIX_ASSERT(bitmap != NULL);
    maxClip = (pxVec2) { .x = bitmap->width, .y = bitmap->height }; 
    if (fpclassify(signedArea) == FP_ZERO)
        return;
    pxFloorVec2(&bbBoxMin); pxFloorVec2(&bbBoxMax);
    bbBoxMin = pxClampVec2(bbBoxMin, minClip, maxClip);
    bbBoxMax = pxClampVec2(bbBoxMax, minClip, maxClip);
    for (unsigned y = bbBoxMin.y; y < bbBoxMax.y; ++y) {
        int hasDrawn = 0;
        for (unsigned x = bbBoxMin.x; x < bbBoxMax.x; ++x) {
            pxVec2 p = {
                x + 0.5, y + 0.5
            };
            double bc[3];
            bc[0] = pxSignedArea(p, b, c) / signedArea;
            bc[1] = pxSignedArea(p, c, a) / signedArea;
            bc[2] = pxSignedArea(p, a, b) / signedArea;
            if ( bc[0] > 0.0 && bc[1] > 0.0 && bc[2] > 0.0 ) {
                pxColorRgba color = pxColorInterpolate(3, colors, bc); 
                unsigned pixelColor = ((unsigned) (color.r))      | 
                                      ((unsigned) (color.g) << 8) |
                                      ((unsigned) (color.b) << 16); 
                hasDrawn = 1; 
                pxSetPixel(bitmap, x, y, pixelColor);
            } else if (hasDrawn) break;
        }
    }
}

#endif
