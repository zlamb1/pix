#ifdef PIX_PLATFORM_WIN32

#include "pix_window_win32.h"
#include "pix_bitmap_win32.h"

pxResult
pxInitBitmap(pxWindow *base, unsigned width, unsigned height, void *pixels, pxBitmap **out)
{
    pxBitmapWin32 *bitmap;
    DIBSECTION section;
    PIX_ASSERT(out != NULL);
    (void) base ;
    bitmap = malloc(sizeof(pxBitmapWin32));
    if ( bitmap == NULL )
        return PIX_ERR_ALLOC;
    memset(bitmap, 0, sizeof(pxBitmapWin32)); 
    if (width == 0)
        width = 1;
    if (height == 0)
        height = 1; 
    bitmap->win32BitmapInfo.bmiHeader.biSize = sizeof(bitmap->win32BitmapInfo.bmiHeader);
    bitmap->win32BitmapInfo.bmiHeader.biPlanes = 1; 
    bitmap->win32BitmapInfo.bmiHeader.biBitCount = 32; 
    bitmap->win32BitmapInfo.bmiHeader.biCompression = BI_RGB; 
    bitmap->win32BitmapInfo.bmiHeader.biWidth = width;
    bitmap->win32BitmapInfo.bmiHeader.biHeight = height; 
    bitmap->win32BitmapInfo.bmiHeader.biSizeImage = width * height * 4;
    // FIXME: currently ignores pixels
    bitmap->base.data = pixels;
    bitmap->base.width = width;
    bitmap->base.height = height;
    bitmap->win32Bitmap = CreateDIBSection(NULL, &bitmap->win32BitmapInfo, DIB_RGB_COLORS, (void **) &bitmap->base.data, NULL, 0);
    if ( bitmap->win32Bitmap == NULL ) {
        free(bitmap); 
        *out = NULL;
        return PIX_ERR_PLATFORM;
    }
    if ( GetObject(bitmap->win32Bitmap, sizeof(DIBSECTION), &section) == 0 ) {
        DeleteObject(bitmap->win32Bitmap);
        free(bitmap);
        *out = NULL;
        return PIX_ERR_PLATFORM;
    }
    bitmap->base.bpp = section.dsBm.bmBitsPixel >> 2;
    bitmap->base.bytesPerPixel = 4; 
    bitmap->base.pitch = section.dsBm.bmWidthBytes; 
    bitmap->base.mask[0] = 0x00FF0000;
    bitmap->base.mask[1] = 0x0000FF00;
    bitmap->base.mask[2] = 0x000000FF;
    bitmap->base.shift[0] = 16;
    bitmap->base.shift[1] = 8;
    bitmap->base.shift[2] = 0;
    *out = &bitmap->base;
    return PIX_SUCCESS;
}

pxResult 
pxBlitBitmap(pxWindow *windowBase, pxBitmap *bitmapBase)
{
    HDC deviceHDC;
    HGDIOBJ deviceBitmap; 
    pxBitmapWin32 *bitmap;
    PIX_GET_WIN32_INSTANCE(windowBase);
    PIX_ASSERT(bitmapBase != NULL);
    bitmap = (pxBitmapWin32 *) bitmapBase; 
    deviceHDC = CreateCompatibleDC(window->win32HDC); 
    deviceBitmap = SelectObject(deviceHDC, bitmap->win32Bitmap);
    BitBlt(
        window->win32HDC, 
        0, 0, 
        bitmap->base.width, bitmap->base.height,
        deviceHDC, 0, 0,
        SRCCOPY
    );
    SelectObject(deviceHDC, deviceBitmap);
    DeleteDC(deviceHDC);
    return PIX_SUCCESS;
}

pxResult
pxDestroyBitmap(pxWindow *baseWindow, pxBitmap *baseBitmap)
{
    pxBitmapWin32 *bitmap;
    PIX_ASSERT(baseBitmap != NULL);
    (void) baseWindow; 
    bitmap = (pxBitmapWin32 *) baseBitmap;
    if ( bitmap->win32Bitmap != NULL )
        DeleteObject(bitmap->win32Bitmap); 
    free(bitmap);
    return PIX_SUCCESS; 
}

#endif
