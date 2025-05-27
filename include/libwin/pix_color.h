#ifndef PIX_COLOR_H
#define PIX_COLOR_H 1

typedef struct 
pxColorRgba
{
    double r, g, b, a; 
} pxColorRgba;

#define __pxMakeOpaqueColor(R, G, B) ( (pxColorRgba) { .r = (R), .g = (G), .b = (B), .a = 255.0 } )
#define pxColor_Black  __pxMakeOpaqueColor(0,   0,   0)
#define pxColor_White  __pxMakeOpaqueColor(255, 255, 255)
#define pxColor_Red    __pxMakeOpaqueColor(255, 0,   0) 
#define pxColor_Yellow __pxMakeOpaqueColor(255, 255, 0)
#define pxColor_Green  __pxMakeOpaqueColor(0,   255, 0)
#define pxColor_Blue   __pxMakeOpaqueColor(0,   0,   255)
#define pxColor_Cyan   __pxMakeOpaqueColor(0,   255, 255)
#define pxColor_Purple __pxMakeOpaqueColor(255, 0,   255)

static inline pxColorRgba 
pxColorInterpolate(unsigned ncomp, pxColorRgba *colors, double *values)
{
    pxColorRgba color = pxColor_Black; 
    for (unsigned i = 0; i < ncomp; i++) {
        color.r += colors[i].r * values[i];
        color.g += colors[i].g * values[i]; 
        color.b += colors[i].b * values[i]; 
    } 
    return color;
}

static inline pxColorRgba
pxColorLerp(pxColorRgba c0, pxColorRgba c1, double t)
{
    double values[2] = {  t, 1 - t };
    pxColorRgba colors[2] = { c0, c1 };
    return pxColorInterpolate(2, colors, values);
}

#endif
