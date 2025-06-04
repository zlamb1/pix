#ifndef PIX_WINDOW_X11_H
#define PIX_WINDOW_X11_H 1

#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include "libwin/pix_window.h"

#include "pix_window_internal.h"

typedef struct 
pxWindowX11
{
    struct pxWindow base;

    struct xcb_connection_t *xcbConnection;
    struct xcb_screen_t *xcbScreen; 
    xcb_window_t xcbWindow; 

    xcb_intern_atom_reply_t *xcbWmProtocolsAtomReply; 
    xcb_intern_atom_reply_t *xcbWmDeleteWindowAtomReply; 

    int x, y; 
    unsigned int width, height; 
    const char *title; 
} pxWindowX11;

#endif
