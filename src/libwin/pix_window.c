#include <string.h>

#include "libbase/pix_assert.h"

#include "libwin/pix_window.h"

#include "pix_window_internal.h"

pxBool 
pxWindowShouldClose(pxWindow *base)
{
    PIX_ASSERT(base != NULL);
    return base->shouldClose; 
}

void *
pxWindowGetUserPointer(pxWindow *base)
{
    PIX_ASSERT(base != NULL);
    return base->userPointer; 
}

void 
pxWindowSetUserPointer(pxWindow *base, void *userPointer)
{
    PIX_ASSERT(base != NULL); 
    base->userPointer = userPointer; 
}

void
pxWindowSetExposeCallback(pxWindow *base, pxWindowExposeCallback callback)
{
    PIX_ASSERT(base != NULL); 
    base->exposeCallback = callback; 
}

void 
pxWindowSetPosCallback(pxWindow *base, pxWindowPosCallback callback)
{
    PIX_ASSERT(base != NULL); 
    base->posCallback = callback; 
}

void 
pxWindowSetSizeCallback(pxWindow *base, pxWindowSizeCallback callback)
{
    PIX_ASSERT(base != NULL); 
    base->sizeCallback = callback; 
}

void 
pxWindowSetCloseCallback(pxWindow *base, pxWindowCloseCallback callback)
{
    PIX_ASSERT(base != NULL); 
    base->closeCallback = callback; 
}

pxResult 
pxWindowSetTitle(pxWindow *base, const char *title)
{
    PIX_ASSERT(base != NULL);
    return pxWindowSetTitleWithLength(base, title, strlen(title)); 
}

pxResult
pxInitFrameBitmap(pxWindow *base, void *pixels, pxBitmap **out)
{
    return pxInitBitmap(base, base->width, base->height, pixels, out);
}
