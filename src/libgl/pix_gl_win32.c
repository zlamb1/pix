#include <windows.h>

#include "libbase/pix_assert.h"

#include "libgl/pix_gl.h"

typedef PROC (WINAPI *PFNWGLGETPROCADDRESS)(LPCSTR);
static PFNWGLGETPROCADDRESS _wglGetProcAddress = NULL;

pxResult 
pxInitGL(void)
{
    // FIXME: think about consildating calls for wgl** functions
    HMODULE module = LoadLibrary("opengl32.dll");
    if ( module == NULL )
        return PIX_ERR_NOT_FOUND;
    _wglGetProcAddress = (PFNWGLGETPROCADDRESS) GetProcAddress(module, "wglGetProcAddress");
    if (_wglGetProcAddress == NULL)
        return PIX_ERR_PLATFORM;
    return PIX_SUCCESS;
}

void *
pxLoadGLFunction(const pxChar *name)
{
    void *f;
    PIX_ASSERT(_wglGetProcAddress != NULL); 
    f = (void *) _wglGetProcAddress(name);
    if ( f == NULL        || 
         f == (void *) -1 || 
         f == (void *)  1 || 
         f == (void *)  2 || 
         f == (void *)  3 
    ) {
        HMODULE module = LoadLibraryA("opengl32.dll");
        f = (void *) GetProcAddress(module, name); 
    }
    return f; 
}
