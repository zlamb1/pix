#include <stdio.h>

#include "pix.h"

#include "libwin/pix_window.h"

static pxBitmap *bitmap; 
static double deltaTime = 1; 

#define ONE_THIRD  (1.0 / 3.0)
#define TWO_THIRDS (2.0 / 3.0)

static inline pxColorRgba 
interpolateColors(pxColorRgba a, pxColorRgba b, pxColorRgba c, double t)
{
    t = fmod(t, 1);
    if ( t <= ONE_THIRD ) {
        return pxColorLerp(a, b, 1.0 - t * 3);
    }  else if ( t <= TWO_THIRDS ) {
        return pxColorLerp(b, c, 1.0 - (t - ONE_THIRD) * 3);
    } else {
        return pxColorLerp(c, a, 1.0 - (t - TWO_THIRDS) * 3);
    }
}

static void 
drawFrame()
{
    static float t = 0;
    double startTime = pxGetTime();
    pxColorRgba colors[3] = {
        interpolateColors(pxColor_Red, pxColor_Green, pxColor_Blue, t),
        interpolateColors(pxColor_Green, pxColor_Blue, pxColor_Red, t),
        interpolateColors(pxColor_Blue, pxColor_Red, pxColor_Green, t)
    };
    pxVec2 a = { bitmap->width * ONE_THIRD, bitmap->height * ONE_THIRD }, 
           b = { bitmap->width * 0.5, bitmap->height * TWO_THIRDS }, 
           c = { bitmap->width * TWO_THIRDS, bitmap->height * ONE_THIRD };
    pxClearColor(bitmap, 0x00000000);
    for ( unsigned i = 0; i < 1; i++ )
        pxDrawTriangle(bitmap, a, b, c, colors);
    t += 1.0f * deltaTime;
}

static void
renderFrame(pxWindow *window)
{
    if ( bitmap != NULL ) {
        drawFrame();
        pxBlitBitmap(window, bitmap);
    }
}

static void 
windowSizeCallback(pxWindow *window, unsigned int width, unsigned int height)
{
    if ( bitmap != NULL )
        pxDestroyBitmap(window, bitmap);
    if ( pxInitFrameBitmap(window, NULL, &bitmap) != PIX_SUCCESS ) {
        pxCloseWindow(window);
        pxExitThread(NULL); 
    }
}

void *
renderThread(void *p)
{
    pxWindow *window = (pxWindow *) p;
    unsigned frames = 0; 
    double fpsStart, fpsCurrent, fpsDelta;
    char title[64];
    if ( pxInitFrameBitmap(window, NULL, &bitmap) != PIX_SUCCESS ) {
        // FIXME: close window
        return NULL; 
    }
    fpsStart = pxGetTime();
    while ( !pxWindowShouldClose(window) ) {
        fpsCurrent = pxGetTime();
        fpsDelta = fpsCurrent - fpsStart;
        frames++;
        if ( fpsDelta >= 1.0 ) {
            fpsStart = fpsCurrent;
            double fps = frames / fpsDelta;
            snprintf(title, sizeof(title), "Win32 Demo - FPS %u/%.2fms [SF]", (unsigned) fps, 1000.0 / fps); 
            pxWindowSetTitle(window, title);
            frames = 0;
        }
        renderFrame(window);
        pxWindowDispatchEvents(window);
        deltaTime = pxGetTime() - fpsCurrent; 
    }
    return NULL;
}

int
main(void)
{
    pxWindow *window;
    if ( pxInit() != PIX_SUCCESS )
        return -1; 
    if ( pxInitWindow(NULL, &window) != PIX_SUCCESS ) 
        return -2;
    pxWindowSetSizeCallback(window, windowSizeCallback);
    pxWindowSetTitle(window, "Win32 Demo [SF]"); 
    pxThread *thread = pxCreateThread(renderThread, window);
    while ( !pxWindowShouldClose(window) ) {
        pxWindowQueueEvents(window, PIX_TRUE);
    }
    pxJoinThread(thread, NULL); 
    pxDestroyBitmap(window, bitmap);
    pxDestroyWindow(window); 
    return 0;
}