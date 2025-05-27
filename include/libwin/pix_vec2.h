#ifndef PIX_VEC2_H
#define PIX_VEC2_H 1

#include <math.h>

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

static inline pxVec2
pxMinVec2(pxVec2 a, pxVec2 b)
{
    return (pxVec2) { 
        .x = a.x < b.x ? a.x : b.x, 
        .y = a.y < b.y ? a.y : b.y
    };
}

static inline pxVec2
pxMaxVec2(pxVec2 a, pxVec2 b)
{
    return (pxVec2) { 
        .x = a.x < b.x ? b.x : a.x, 
        .y = a.y < b.y ? b.y : a.y
    };
}

static inline pxVec2
pxClampVec2(pxVec2 v, pxVec2 min, pxVec2 max)
{
    return (pxVec2) {
        .x = v.x <  min.x ? min.x :
             v.x >= max.x ? max.x - 1 : v.x,
        .y = v.y <  min.y ? min.y :
             v.y >= max.y ? max.y - 1 : v.y,
    };
}

static inline float 
pxClampf(float val, float min, float max)
{
    return val <  min ? min :
           val >= max ? max - 1 : val;  
}

#endif
