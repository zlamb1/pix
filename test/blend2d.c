#include <blend2d.h>
#include <blend2d/context.h>
#include <stdlib.h>

#include "libperf/pix_clock.h"
#include "libwin/pix_bitmap.h"
#include "libwin/pix_window.h"

/**
 * Blend2D Test
 */

static struct pxBitmap *bitmap; 

static BLImageCore img;
static BLContextCore ctx;

#define BL_CALL(CALL) \
    do { \
        BLResult result = (CALL); \
        if ((result) != BL_SUCCESS) { \
            printf("blCallFailed: %u", result); \
            exit(-1); \
        } \
    } while (0)

static inline void 
initB2D(unsigned width, unsigned height)
{
    BL_CALL(blImageInitAsFromData(&img, width, height, BL_FORMAT_XRGB32, 
    bitmap->data, bitmap->pitch, BL_DATA_ACCESS_RW, NULL, NULL));
    BL_CALL(blContextInitAs(&ctx, &img, NULL));
}

static void
winExposeCallback(struct pxWindow *win)
{
    pxWindowBlitBitmap(win, bitmap); 
}

static void 
winSizeCallback(struct pxWindow *win, unsigned int width, unsigned int height)
{
    bitmap = pxWindowGetSizedBitmap(win, width, height, NULL);
    blContextReset(&ctx);
    blImageReset(&img); 
    initB2D(bitmap->width, bitmap->height); 
}

int
main(void) 
{
    struct pxWindow *win;

    win = pxWindowInit();
    pxWindowSetExposeCallback(win, winExposeCallback);
    pxWindowSetSizeCallback(win, winSizeCallback);
    pxWindowSetTitle(win, "Blend2D Demo");
    bitmap = pxWindowGetBitmap(win, NULL);

    initB2D(bitmap->width, bitmap->height); 

    while (!pxWindowShouldClose(win)) {
        pxClearColor(bitmap, 0x00FF00FF);
        PIX_CLOCK_START();
        BL_CALL(blContextFillAllRgba32(&ctx, 0xFFFF0000));
        PIX_CLOCK_END();
        PIX_CLOCK_PRINT();
        pxWindowBlitBitmap(win, bitmap); 
        pxWindowPollEvents(win);
    }

    /* clean up */
    blContextDestroy(&ctx);
    blImageDestroy(&img);
    pxDestroyWindow(win);

    return 0; 
}
