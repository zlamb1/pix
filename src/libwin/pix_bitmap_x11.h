#ifndef PIX_BITMAP_X11_H
#define PIX_BITMAP_X11_H 1

#include <stdio.h>
#include <string.h>

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include <xcb/xproto.h>

#include "pix_assert.h"

#include "pix_window_x11.h"

#include "libwin/pix_bitmap.h"

struct 
pxBitmapX11
{
    struct pxBitmap base; 

    xcb_image_t *image; 
    xcb_gcontext_t gc; 
};

#define CHECK_MASK(mask) ((mask) == 0xFF || (mask) == 0xFF00 || (mask) == 0xFF0000)
#define MASK_TO_SHIFT(mask) ((mask) == 0xFF ? 0 : ((mask) == 0xFF00 ? 8 : 16)); 

static struct pxBitmapX11 * 
pix_bitmap_x11_init_with_base(struct pxWindowX11 *win, unsigned char depth, unsigned width, unsigned height, void *base)
{
    unsigned gc_mask, gc_values[1]; 
    xcb_depth_iterator_t depth_iter; 
    xcb_visualtype_iterator_t visualtype_iter; 
    xcb_visualtype_t *visualtype = NULL; 
    struct pxBitmapX11 *bitmap;

    PIX_ASSERT(win != NULL)

    if (!width)
        width = 1;

    if (!height)
        height = 1; 

    bitmap = malloc(sizeof(struct pxBitmapX11));
    if (bitmap == NULL)
        return NULL; 

    memset(bitmap, 0, sizeof(struct pxBitmapX11)); 

    gc_mask = XCB_GC_FOREGROUND;
    gc_values[0] = 0; 

    bitmap->gc = xcb_generate_id(win->xcbConnection);
    xcb_create_gc(win->xcbConnection, bitmap->gc, win->xcbWindow, gc_mask, gc_values);

    bitmap->image = xcb_image_create_native(
        win->xcbConnection, 
        width, 
        height, 
        XCB_IMAGE_FORMAT_Z_PIXMAP, 
        depth, 
        base, 
        0, 
        NULL);

    bitmap->base.data = bitmap->image->base; 

    bitmap->base.width = width;
    bitmap->base.height = height; 
    bitmap->base.pitch = bitmap->image->stride; 

    bitmap->base.bpp = 8;
    bitmap->base.bytesPerPixel = 4; 

    depth_iter = xcb_screen_allowed_depths_iterator(win->xcbScreen); 
    for (; !visualtype && depth_iter.rem; xcb_depth_next(&depth_iter)) {
        xcb_depth_t *allowed_depth = depth_iter.data;
        visualtype_iter = xcb_depth_visuals_iterator(allowed_depth); 
        for (; visualtype_iter.rem; xcb_visualtype_next(&visualtype_iter)) {
            if (visualtype_iter.data->visual_id == win->xcbScreen->root_visual) {
                visualtype = visualtype_iter.data; 
                break;
            }
        }
    }

    PIX_ASSERT(visualtype); 
    bitmap->base.mask[0] = visualtype->red_mask; 
    bitmap->base.mask[1] = visualtype->green_mask;
    bitmap->base.mask[2] = visualtype->blue_mask;

    PIX_ASSERT(CHECK_MASK(bitmap->base.mask[0]));
    PIX_ASSERT(CHECK_MASK(bitmap->base.mask[1]));
    PIX_ASSERT(CHECK_MASK(bitmap->base.mask[2]));

    bitmap->base.shift[0] = MASK_TO_SHIFT(bitmap->base.mask[0]); 
    bitmap->base.shift[1] = MASK_TO_SHIFT(bitmap->base.mask[1]); 
    bitmap->base.shift[2] = MASK_TO_SHIFT(bitmap->base.mask[2]);

    return bitmap; 
}

static inline struct pxBitmapX11 * 
pix_bitmap_x11_init(struct pxWindowX11 *win, unsigned char depth, unsigned width, unsigned height)
{
    return pix_bitmap_x11_init_with_base(win, depth, width, height, NULL); 
}

static inline void
pix_bitmap_x11_destroy(struct pxBitmap *_bitmap)
{
    struct pxBitmapX11 *bitmap;
    PIX_ASSERT(_bitmap != NULL);
    bitmap = (struct pxBitmapX11 *) _bitmap; 
    xcb_image_destroy(bitmap->image); 
    free(bitmap); 
} 

#endif
