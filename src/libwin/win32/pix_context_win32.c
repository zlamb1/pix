#include <windows.h>

#include "libwin/pix_context.h"

#include "pix_window_win32.h"

typedef HGLRC (WINAPI *PFNWGLCREATECONTEXT)(HDC);
static PFNWGLCREATECONTEXT _wglCreateContext = NULL;
typedef BOOL (WINAPI *PFNWGLMAKECURRENT)(HDC, HGLRC);
static PFNWGLMAKECURRENT _wglMakeCurrent = NULL;

pxResult
pxInitContext(void)
{
    HMODULE module = LoadLibraryA("opengl32.dll");
    if ( module == NULL )
        return PIX_ERR_NOT_FOUND;
    _wglCreateContext = (PFNWGLCREATECONTEXT) GetProcAddress(module, "wglCreateContext");
    _wglMakeCurrent   = (PFNWGLMAKECURRENT)   GetProcAddress(module, "wglMakeCurrent");
    if ( _wglCreateContext == NULL || _wglMakeCurrent == NULL )
        return PIX_ERR_PLATFORM;
    return PIX_SUCCESS;
}

pxResult
pxCreateContext(pxWindow *base)
{
    PIXELFORMATDESCRIPTOR pfd;
    pxInt pixelFormat;
    PIX_GET_WIN32_INSTANCE(base);
    PIX_ASSERT(_wglCreateContext != NULL); 
    PIX_ASSERT(window->win32HWND != NULL); 
    PIX_ASSERT(window->win32HDC != NULL); 
    // FIXME: use extensions for more config
    // See: https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)#Create_the_Context
    pfd = (PIXELFORMATDESCRIPTOR) {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    if ( ( pixelFormat = ChoosePixelFormat(window->win32HDC, &pfd) ) == 0 ) {
        if (GetLastError() != 0)
            return PIX_ERR_PLATFORM;
        return PIX_ERR_NOT_FOUND; 
    }
    if ( SetPixelFormat(window->win32HDC, pixelFormat, &pfd) == FALSE )
        return PIX_ERR_PLATFORM;
    window->win32OpenGLCtx = _wglCreateContext(window->win32HDC);
    if ( window->win32OpenGLCtx == NULL )
        return PIX_ERR_PLATFORM; 
    return PIX_SUCCESS;
}

pxResult
pxMakeContextCurrent(pxWindow *base)
{
    pxWindowWin32 *window = (pxWindowWin32 *) base;
    PIX_ASSERT(_wglMakeCurrent != NULL); 
    if ( window == NULL ) {
        if ( _wglMakeCurrent(NULL, NULL) == FALSE )
            return PIX_ERR_PLATFORM;
        return PIX_SUCCESS;
    }
    PIX_ASSERT(window->win32HDC != NULL);
    PIX_ASSERT(window->win32OpenGLCtx != NULL); 
    if ( _wglMakeCurrent(window->win32HDC, window->win32OpenGLCtx) == FALSE )
        return PIX_ERR_PLATFORM;
    return PIX_SUCCESS; 
}
