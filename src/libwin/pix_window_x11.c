#include "libwin/pix_bitmap.h"
#include "pix_window_internal.h"
#ifdef PIX_PLATFORM_LINUX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pix_window_x11.h"
#include "pix_bitmap_x11.h"

#include "pix_assert.h"

struct pxWindow * 
pxWindowInit(void)
{
    const xcb_setup_t *setup; 
    xcb_screen_iterator_t screen_iter; 

    int window_mask, window_values[3]; 
    xcb_void_cookie_t map_window_cookie;
    xcb_generic_error_t *error;

    const char *win_wm_protocols_atom;
    const char *win_wm_delete_atom; 

    xcb_intern_atom_cookie_t win_wm_protocols_cookie;
    xcb_intern_atom_cookie_t win_wm_delete_cookie; 

    struct pxWindowX11 *win = malloc(sizeof(struct pxWindowX11));
    memset(win, 0, sizeof(struct pxWindowX11));

    if (win == NULL)
        return NULL; 
    
    win->xcbConnection = xcb_connect(NULL, NULL); 
    if (xcb_connection_has_error(win->xcbConnection)) {
        // FIXME: return error to user?
        free(win);
        return NULL; 
    }

    setup = xcb_get_setup(win->xcbConnection);
    screen_iter = xcb_setup_roots_iterator(setup);
    win->xcbScreen = screen_iter.data;

    if (win->xcbScreen == NULL) {
        xcb_disconnect(win->xcbConnection); 
        free(win); 
        return NULL;
    }

    window_mask = XCB_CW_BACK_PIXEL | XCB_CW_BIT_GRAVITY | XCB_CW_EVENT_MASK; 
    
    window_values[0] = win->xcbScreen->white_pixel;
    window_values[1] = XCB_GRAVITY_STATIC; 
    window_values[2] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY; 

    win->xcbWindow = xcb_generate_id(win->xcbConnection);

    xcb_create_window(
        win->xcbConnection,
        0,
        win->xcbWindow,
        win->xcbScreen->root,
        0,
        0,
        500,
        500,
        10,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        win->xcbScreen->root_visual,
        window_mask,
        window_values
    );

    map_window_cookie = xcb_map_window_checked(win->xcbConnection, win->xcbWindow);

    if ((error = xcb_request_check(win->xcbConnection, map_window_cookie))) {
        xcb_disconnect(win->xcbConnection); 
        free(win); 
        return NULL;
    }

    win_wm_protocols_atom = "WM_PROTOCOLS";
    win_wm_delete_atom = "WM_DELETE_WINDOW";
    
    win_wm_protocols_cookie = xcb_intern_atom(win->xcbConnection, 0, strlen(win_wm_protocols_atom), win_wm_protocols_atom);
    win->xcbWmProtocolsAtomReply = xcb_intern_atom_reply(win->xcbConnection, win_wm_protocols_cookie, 0); 

    win_wm_delete_cookie = xcb_intern_atom(win->xcbConnection, 0, strlen(win_wm_delete_atom), win_wm_delete_atom);
    win->xcbWmDeleteWindowAtomReply = xcb_intern_atom_reply(win->xcbConnection, win_wm_delete_cookie, 0); 
    
    xcb_change_property(win->xcbConnection,
                      XCB_PROP_MODE_REPLACE,
                      win->xcbWindow,
                      win->xcbWmProtocolsAtomReply->atom,
                      XCB_ATOM_ATOM,
                      32,
                      1,
                      &win->xcbWmDeleteWindowAtomReply->atom);

    xcb_flush(win->xcbConnection);

    return &win->base; 
}

static void 
pxWindowHandleEvent(struct pxWindowX11 *win, xcb_generic_event_t *e)
{
    switch (e->response_type & ~0x80)
    {
        case 0: {
            xcb_generic_error_t *error = (xcb_generic_error_t *) e; 
            printf("pix_window_x11: error -> %i\n", error->error_code); 
            break;
        }
        case XCB_EXPOSE: {
            if (win->base.exposeCallback != NULL) {
                win->base.exposeCallback(&win->base); 
            }

            break;
        }
        case XCB_CONFIGURE_NOTIFY: {
            xcb_configure_notify_event_t *event = (xcb_configure_notify_event_t *) e;

            if (event->x != win->x || event->y != win->y) {
                win->x = event->x;
                win->y = event->y; 
                if (win->base.posCallback != NULL) {
                    win->base.posCallback(&win->base, win->x, win->y); 
                }
            }

            if (event->width != win->width || event->height != win->height) {
                win->width = event->width;
                win->height = event->height; 
                if (win->base.sizeCallback != NULL) {
                    win->base.sizeCallback(&win->base, win->width, win->height); 
                }
            }

            break;
        }
        case XCB_CLIENT_MESSAGE: {
            xcb_client_message_event_t *event = (xcb_client_message_event_t *) e; 
            if (event->data.data32[0] == win->xcbWmDeleteWindowAtomReply->atom) {
                win->base.shouldClose = 1; 
                if (win->base.closeCallback != NULL) {
                    win->base.closeCallback(&win->base);
                }
            }

            break;
        }
    }

    free(e); 
}

void
pxWindowPollEvents(struct pxWindow *_win)
{
    struct pxWindowX11 *win = (struct pxWindowX11 *) _win;
    xcb_generic_event_t *e;

    while ((e = xcb_poll_for_event(win->xcbConnection)) != NULL)
        pxWindowHandleEvent(win, e); 
}

void 
pxWindowWaitEvents(struct pxWindow *_win)
{
    struct pxWindowX11 *win = (struct pxWindowX11 *) _win;
    xcb_generic_event_t *e;

    while (!pxWindowShouldClose(_win) && (e = xcb_wait_for_event(win->xcbConnection)) != NULL)
        pxWindowHandleEvent(win, e);
}

struct pxWindowPos 
pxWindowGetPos(struct pxWindow *_win)
{
    struct pxWindowX11 *win;
    PIX_ASSERT(_win != NULL);
    win = (struct pxWindowX11 *) _win;

    return (struct pxWindowPos) {
        .x = win->x,
        .y = win->y
    };
}

void 
pxWindowSetPos(struct pxWindow *win, int x, int y)
{
    (void) win;
    (void) x;
    (void) y;
}

struct pxWindowSize
pxWindowGetSize(struct pxWindow *_win)
{
    struct pxWindowX11 *win;
    PIX_ASSERT(_win != NULL);
    win = (struct pxWindowX11 *) _win;
    
    return (struct pxWindowSize) {
        .width = win->width,
        .height = win->height
    };
}

void 
pxWindowSetSize(struct pxWindow *win, unsigned int width, unsigned int height)
{
    (void) win;
    (void) width;
    (void) height;
}

const char *
pxWindowGetTitle(struct pxWindow *_win)
{
    struct pxWindowX11 *win;
    PIX_ASSERT(_win != NULL);
    win = (struct pxWindowX11 *) _win;
    return win->title; 
}

void
pxWindowSetTitleWithLength(struct pxWindow *_win, const char *title, unsigned int len)
{
    struct pxWindowX11 *win;
    PIX_ASSERT(_win != NULL);
    win = (struct pxWindowX11 *) _win;

    xcb_change_property(
        win->xcbConnection, 
     XCB_PROP_MODE_REPLACE, 
   win->xcbWindow,
 XCB_ATOM_WM_NAME, 
     XCB_ATOM_STRING, 
   8,
 len, 
     title
     );
     
    xcb_flush(win->xcbConnection); 
    win->title = title;
}

struct pxBitmap *
pxWindowGetBitmap(struct pxWindow *_win, void *base)
{
    struct pxWindowX11 *win;
    PIX_ASSERT(_win);
    win = (struct pxWindowX11 *) _win;
    return pxWindowGetSizedBitmap(_win, win->width, win->height, base);
}

struct pxBitmap *
pxWindowGetSizedBitmap(struct pxWindow *_win, unsigned width, unsigned height, void *base)
{
    struct pxWindowX11 *win;
    struct pxBitmapX11 *bitmap;

    PIX_ASSERT(_win);
    win = (struct pxWindowX11 *) _win; 
    
    bitmap = pix_bitmap_x11_init_with_base(
        win,
        win->xcbScreen->root_depth, 
        width, 
        height, 
        base
    );

    return &bitmap->base;
}

void 
pxWindowBlitBitmap(struct pxWindow *_win, struct pxBitmap *_bitmap)
{
    struct pxWindowX11 *win;
    struct pxBitmapX11 *bitmap;

    PIX_ASSERT(_win != NULL);
    PIX_ASSERT(_bitmap != NULL); 

    win = (struct pxWindowX11 *) _win; 
    bitmap = (struct pxBitmapX11 *) _bitmap;

    xcb_image_put(
        win->xcbConnection,
        win->xcbWindow,
        bitmap->gc,
        bitmap->image,
        0,
        0,
        0
    );

    xcb_flush(win->xcbConnection); 
}

void 
pxDestroyWindow(struct pxWindow *_win)
{
    struct pxWindowX11 *win; 
    PIX_ASSERT(_win != NULL); 
    win = (struct pxWindowX11 *) _win; 
    xcb_disconnect(win->xcbConnection); 
    free(win); 
}

#endif
