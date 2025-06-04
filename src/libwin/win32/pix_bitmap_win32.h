#ifndef PIX_BITMAP_WIN32
#define PIX_BITMAP_WIN32 1

#include <windows.h>

#include "libwin/pix_bitmap.h"

typedef struct
pxBitmapWin32
{
    pxBitmap base;
    BITMAPINFO win32BitmapInfo;
    HBITMAP win32Bitmap; 
} pxBitmapWin32;

#endif
