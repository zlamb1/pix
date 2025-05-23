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
    /* check pointer alignment */
    if ( (((uintptr_t) ip ) & 15) == 0 ) {
        while (cnt--) {
            _mm_store_si128((__m128i *) ip, vec);
            ip += 4; 
        }
    } else {
        while (cnt--) {
            _mm_storeu_si128((__m128i *) ip, vec);
            ip += 4; 
        }
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
pxClearHLine(struct pxBitmap *bitmap, unsigned y, unsigned x0, unsigned x1, unsigned color)
{
    unsigned *data, fcolor;

    PIX_ASSERT(bitmap != NULL);
    PIX_ASSERT(x0 <= x1);
    PIX_ASSERT(x1 < bitmap->width); 
    PIX_ASSERT(y < bitmap->height);

    data = (void *) bitmap->data;
    data += (bitmap->pitch >> 2) * y + x0; 
    fcolor = ((color & 0xFF)           << bitmap->shift[0]) |
             ((color & 0xFF00)   >>  8 << bitmap->shift[1]) |
             ((color & 0xFF0000) >> 16 << bitmap->shift[2]); 

    pxMemset32(data, fcolor, x1 - x0); 
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

typedef struct
pxVec2
{
    float x, y; 
} pxVec2;    

static inline pxVec2
pxAddVec2(pxVec2 a, pxVec2 b)
{
    return (pxVec2) { a.x + b.x, a.y + b.y };
}

static inline pxVec2
pxSubtractVec2(pxVec2 b, pxVec2 a)
{
    return (pxVec2) { b.x - a.x, b.y - a.y }; 
}

static inline pxVec2
pxScalarDivideVec2(pxVec2 a, float s)
{
    if (FP_ZERO == fpclassify(s))
        return a; 
    return (pxVec2) { a.x / s, a.y / s };
}

static inline void
pxFloorVec2(pxVec2 *a)
{
    a->x = (int) a->x;
    a->y = (int) a->y;
}

static inline float 
pxClampf(float val, float min, float max)
{
    return val <  min ? min :
           val >= max ? max - 1 : val;  
}

static inline void 
pxDrawTriangle(struct pxBitmap *bitmap, pxVec2 a, pxVec2 b, pxVec2 c, unsigned color)
{
    pxVec2 tmp, p0, p1, d0, d1, d2;
    int steps[3];
    PIX_ASSERT(bitmap != NULL);
    pxFloorVec2(&a); pxFloorVec2(&b); pxFloorVec2(&c); 
    /* order by y-coordinate */
    if (a.y > b.y) {
        tmp = a;
        a = b;
        b = tmp;
    }
    if (b.y > c.y) {
        tmp = b;
        b = c;
        c = tmp;
    }
    if (a.y > b.y) {
        tmp = a;
        a = b;
        b = tmp;
    }
    if ((int) b.y == (int) c.y && b.x < c.x) {
        tmp = b;
        b = c;
        c = tmp;
    }
    p0.x = p1.x = a.x;
    p0.y = p1.y = a.y;
    d0 = pxSubtractVec2(b, a); d1 = pxSubtractVec2(c, a); d2 = pxSubtractVec2(c, b);
    steps[0] = d0.y; steps[1] = d0.y + d2.y; steps[2] = d2.y;
    d0 = pxScalarDivideVec2(d0, steps[0]); 
    d1 = pxScalarDivideVec2(d1, steps[1]);
    d2 = pxScalarDivideVec2(d2, steps[2]);
    if (!steps[0])
        p0 = b;
    while (steps[0]--) {
        if (p0.y >= 0 && p0.y < bitmap->height)
            pxClearHLine(bitmap, p0.y, pxClampf(p1.x, 0, bitmap->width), pxClampf(p0.x, 0, bitmap->width), color);
        p0 = pxAddVec2(p0, d0);
        p1 = pxAddVec2(p1, d1); 
    }
    while (steps[2]-- >= 0) {
        if (p0.y >= 0 && p0.y < bitmap->height)
            pxClearHLine(bitmap, p0.y, pxClampf(p1.x, 0, bitmap->width), pxClampf(p0.x, 0, bitmap->width), color);
        p0 = pxAddVec2(p0, d2);
        p1 = pxAddVec2(p1, d1); 
    }
}

#endif
