#include <time.h>
#include <stdio.h>

#include "libperf/pix_clock.h"
#include "libwin/pix_bitmap.h"
#include "libwin/pix_window.h"

static struct pxBitmap *bitmap; 

static inline void 
drawBitmap(void)
{
    PIX_CLOCK_START();
    pxClearColor(bitmap, 0x000000FF);
    PIX_CLOCK_END();
    PIX_CLOCK_PRINT();
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
