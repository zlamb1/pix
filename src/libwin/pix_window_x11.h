#ifndef PIX_WINDOW_X11_H
#define PIX_WINDOW_X11_H

#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include "pix_window_internal.h"

#include "libwin/pix_window.h"

struct 
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
};

#endif
