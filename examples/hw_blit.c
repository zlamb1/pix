#include <stdio.h>

#include "pix.h"

#include "libwin/pix_window.h"

static pxBitmap *bitmap; 
static double deltaTime = 0; 

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
windowSizeCallback(pxWindow *window, pxUnsigned width, pxUnsigned height)
{
    glViewport(0, 0, width, height);
}

static void 
drawFrame()
{
    static double t = 0;
    pxColorRgba colors[3] = {
        interpolateColors(pxColor_Red, pxColor_Green, pxColor_Blue, t),
        interpolateColors(pxColor_Green, pxColor_Blue, pxColor_Red, t),
        interpolateColors(pxColor_Blue, pxColor_Red, pxColor_Green, t)
    };
    glClear(GL_COLOR_BUFFER_BIT);
    /*glBegin(GL_TRIANGLES);
    glColor3f(colors[0].r / 255.0, colors[0].g / 255.0, colors[0].b / 255.0); 
    glVertex3f(-0.5, -0.5, 0.0);
    glColor3f(colors[1].r / 255.0, colors[1].g / 255.0, colors[1].b / 255.0); 
    glVertex3f( 0.0,  0.5, 0.0);
    glColor3f(colors[2].r / 255.0, colors[2].g / 255.0, colors[2].b / 255.0); 
    glVertex3f( 0.5, -0.5, 0.0);
    glEnd();*/
    t += deltaTime; 
}

static void *
renderThread(void *arg)
{
    pxWindow *window = (pxWindow *) arg;
    unsigned frames = 0; 
    double fpsStart, fpsCurrent, fpsDelta;
    char title[32];
    if ( pxMakeContextCurrent(window) != PIX_SUCCESS ) {
        pxCloseWindow(window);
        pxExitThread(NULL);
    }
    pxLoadGLFunctions();
    fpsStart = pxGetTime();
    while ( !pxWindowShouldClose(window) ) {
        fpsCurrent = pxGetTime();
        fpsDelta = fpsCurrent - fpsStart;
        frames++;
        if ( fpsDelta >= 1.0 ) {
            fpsStart = fpsCurrent;
            double fps = frames / fpsDelta;
            snprintf(title, 32, "Win32 Demo - FPS %u/%.2fms", (unsigned) fps, 1000.0 / fps); 
            pxWindowSetTitle(window, title);
            frames = 0;
        }
        drawFrame();
        pxWindowDispatchEvents(window);
        pxWindowSwapBuffers(window);
        deltaTime = pxGetTime() - fpsCurrent; 
    }
    return NULL;
}

int
main(void)
{
    pxWindowTags windowTags = { .tags = PIX_WINDOW_TAG_GL_CONTEXT };
    pxWindow *window;
    pxThread *thread;
    if ( pxInit() != PIX_SUCCESS )
        return -1; 
    if ( pxInitWindow(&windowTags, &window) != PIX_SUCCESS ) 
        return -2;\
    pxWindowSetSizeCallback(window, windowSizeCallback);
    pxWindowSetTitle(window, "Win32 Demo"); 
    thread = pxCreateThread(renderThread, window);
    while ( !pxWindowShouldClose(window) )
        pxWindowQueueEvents(window, PIX_TRUE);
    pxJoinThread(thread, NULL);
    pxDestroyWindow(window); 
    return 0;
}