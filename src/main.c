#include <sys/types.h>
#include <time.h>
#include <stdio.h>

#include "libperf/pix_clock.h"
#include "libwin/pix_bitmap.h"
#include "libwin/pix_window.h"

static struct pxBitmap *bitmap; 

static inline void 
drawBitmap(void)
{
    static float t = 0; 
    pxVec2 a = { 100, 300 }, b = { 200, 150 * fabs(cos(t)) }, c = { 350, 300 };
    PIX_CLOCK_START();
    pxClearColor(bitmap, 0x0000FF00);
    pxDrawTriangle(bitmap, a, b, c, 0x00FF0000);
    PIX_CLOCK_END();
    PIX_CLOCK_PRINT();
    t += 0.001f;
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
