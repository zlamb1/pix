#include <string.h>

#include "pix_assert.h"

#include "pix_window_internal.h"

#include "libwin/pix_window.h"

int 
pxWindowShouldClose(struct pxWindow *win)
{
    PIX_ASSERT(win != NULL);
    return win->shouldClose; 
}

void *
pxWindowGetUserPointer(struct pxWindow *win)
{
    PIX_ASSERT(win != NULL);
    return win->userPointer; 
}

void 
pxWindowSetUserPointer(struct pxWindow *win, void *userPointer)
{
    PIX_ASSERT(win != NULL); 
    win->userPointer = userPointer; 
}

void
pxWindowSetExposeCallback(struct pxWindow *win, pxWindowExposeCallback callback)
{
    PIX_ASSERT(win != NULL); 
    win->exposeCallback = callback; 
}

void 
pxWindowSetPosCallback(struct pxWindow *win, pxWindowPosCallback callback)
{
    PIX_ASSERT(win != NULL); 
    win->posCallback = callback; 
}

void 
pxWindowSetSizeCallback(struct pxWindow *win, pxWindowSizeCallback callback)
{
    PIX_ASSERT(win != NULL); 
    win->sizeCallback = callback; 
}

void 
pxWindowSetCloseCallback(struct pxWindow *win, pxWindowCloseCallback callback)
{
    PIX_ASSERT(win != NULL); 
    win->closeCallback = callback; 
}

void 
pxWindowSetTitle(struct pxWindow *win, const char *title)
{
    PIX_ASSERT(win != NULL);
    pxWindowSetTitleWithLength(win, title, strlen(title)); 
}
