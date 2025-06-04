#ifndef PIX_WINDOW_WIN32_H
#define PIX_WINDOW_WIN32_H 1

#include <windows.h>

#include "libwin/pix_window.h"

#include "libthread/pix_thread.h"

#include "../pix_window_internal.h"

#define PIX_GET_WIN32_INSTANCE(W) \
    pxWindowWin32 *window; \
    PIX_ASSERT((W) != NULL); \
    window = ( (pxWindowWin32 *) (W) )

typedef struct 
pxWindowWin32
{
    pxWindow base; 
    HWND win32HWND; 
    HDC win32HDC;
    HGLRC win32OpenGLCtx;
} pxWindowWin32;

#endif
