#include "include/core/SkColor.h"
#include <stdlib.h>

#include <include/core/SkCanvas.h>
#include <include/core/SkData.h>
#include <include/core/SkImage.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>
#include <include/core/SkStream.h>
#include <include/core/SkSurface.h>
#include <include/core/SkVertices.h>

extern "C" {
#include "libperf/pix_clock.h"
#include "libwin/pix_bitmap.h"
#include "libwin/pix_window.h"
}

/**
 * Skia Test
 */

static struct pxBitmap *bitmap; 

static sk_sp<SkSurface> rasterSurface;

static inline void
initSkia()
{
    SkImageInfo info = SkImageInfo::MakeN32(bitmap->width, bitmap->height, SkAlphaType::kUnpremul_SkAlphaType);
    rasterSurface = SkSurfaces::WrapPixels(info, &bitmap->data[0], bitmap->pitch);
}

static inline void
drawFrame()
{
    SkPaint paint;
    auto canvas = rasterSurface->getCanvas(); 
    PIX_CLOCK_START();
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(SK_ColorWHITE);
    SkPoint positions[] = {
        {100, 300},
        {200, 150},
        {350, 300}
    };
    SkColor colors[] = {
        SK_ColorWHITE,
        SK_ColorWHITE,
        SK_ColorWHITE
    };
    sk_sp<SkVertices> vertices = SkVertices::MakeCopy(
        SkVertices::kTriangles_VertexMode,
        3,
        positions,
        nullptr, 
        colors,
        0,
        nullptr
    );
    for (unsigned i = 0; i < 10000; ++i)
        canvas->drawVertices(vertices.get(), SkBlendMode::kDst, paint);
    PIX_CLOCK_END();
    PIX_CLOCK_PRINT();
}

static void
winExposeCallback(struct pxWindow *win)
{
}

static void 
winSizeCallback(struct pxWindow *win, unsigned int width, unsigned int height)
{
    bitmap = pxWindowGetSizedBitmap(win, width, height, NULL);
    initSkia(); 
}

int
main(void) 
{
    struct pxWindow *win;

    win = pxWindowInit();
    pxWindowSetExposeCallback(win, winExposeCallback);
    pxWindowSetSizeCallback(win, winSizeCallback);
    pxWindowSetTitle(win, "Skia Demo");
    bitmap = pxWindowGetBitmap(win, NULL);

    initSkia();

    while (!pxWindowShouldClose(win)) {
        drawFrame(); 
        pxWindowBlitBitmap(win, bitmap); 
        pxWindowPollEvents(win);
    }

    /* clean up */
    pxDestroyWindow(win);

    return 0; 
}
