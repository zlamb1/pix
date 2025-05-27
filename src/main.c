#include <sys/types.h>
#include <time.h>
#include <stdio.h>

#include "libperf/pix_clock.h"
#include "libwin/pix_bitmap.h"
#include "libwin/pix_color.h"
#include "libwin/pix_window.h"

static struct pxBitmap *bitmap; 

#define ONE_THIRD  (1.0 / 3.0)
#define TWO_THIRDS (2.0 / 3.0)

static inline pxColorRgba 
interpolateColors(pxColorRgba a, pxColorRgba b, pxColorRgba c, double t)
{
    t = fmod(t, 1);
    if (t <= ONE_THIRD) {
        return pxColorLerp(a, b, 1.0 - t * 3);
    }  else if (t <= TWO_THIRDS) {
        return pxColorLerp(b, c, 1.0 - (t - ONE_THIRD) * 3);
    } else {
        return pxColorLerp(c, a, 1.0 - (t - TWO_THIRDS) * 3);
    }
}

static void 
drawBitmap(void)
{
    static float t = 0;
    pxColorRgba colors[3] = {
        interpolateColors(pxColor_Red, pxColor_Green, pxColor_Blue, t),
        interpolateColors(pxColor_Green, pxColor_Blue, pxColor_Red, t),
        interpolateColors(pxColor_Blue, pxColor_Red, pxColor_Green, t)
    };
    pxVec2 a = { 100, 300 }, b = { 200, 150 }, c = { 350, 300 };
    PIX_CLOCK_START();
    pxClearColor(bitmap, 0x00000000);
    for (unsigned i = 0; i < 1; i++)
        pxDrawTriangle(bitmap, a, b, c, colors);
    PIX_CLOCK_END();
    PIX_CLOCK_PRINT();
    t += 0.0001f;
}

static void
winExposeCallback(struct pxWindow *win)
{
    drawBitmap();
    pxWindowBlitBitmap(win, bitmap); 
}

static void 
winSizeCallback(struct pxWindow *win, unsigned int width, unsigned int height)
{
    bitmap = pxWindowGetSizedBitmap(win, width, height, NULL);
}

int
main(void) 
{
    struct pxWindow *win;

    win = pxWindowInit();
    pxWindowSetExposeCallback(win, winExposeCallback);
    pxWindowSetSizeCallback(win, winSizeCallback);

    pxWindowSetTitle(win, "X11 Demo");

    bitmap = pxWindowGetBitmap(win, NULL);
    
    while (!pxWindowShouldClose(win)) {
        drawBitmap();
        pxWindowBlitBitmap(win, bitmap); 
        pxWindowPollEvents(win);
    }

    /* clean up */
    pxDestroyWindow(win);

    return 0; 
}
