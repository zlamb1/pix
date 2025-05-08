#include <chrono>
#include <iostream>
#include <stdlib.h>

#include <xcb/xcb.h>

#include "window.h"

Window::Window() : m_Width(500), m_Height(500) {
    m_Connection = xcb_connect(NULL, NULL);
    m_Window = xcb_generate_id(m_Connection);

    const xcb_setup_t      *setup  = xcb_get_setup(m_Connection);
    xcb_screen_iterator_t   iter   = xcb_setup_roots_iterator(setup);
    m_Screen = iter.data;

    uint32_t     windowMask      = XCB_CW_BACK_PIXEL | XCB_CW_BIT_GRAVITY | XCB_CW_EVENT_MASK;
    uint32_t     windowValues[3] = { m_Screen->black_pixel, XCB_GRAVITY_STATIC, XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY };

    xcb_create_window(m_Connection,    
                    0,
                    m_Window,
                    m_Screen->root,
                    0, 0,
                    150, 150,
                    10,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    m_Screen->root_visual,
                    windowMask, windowValues);
    
    xcb_map_window(m_Connection, m_Window);

    xcb_flush(m_Connection);

    uint32_t gcMask = XCB_GC_FOREGROUND;
    uint32_t gcValues[1] = { 0 };

    m_GC = xcb_generate_id(m_Connection);
    xcb_create_gc(m_Connection, m_GC, m_Window, gcMask, gcValues);

    CreateCanvas(); 
}

Window::~Window() {
    if (m_Canvas != nullptr) {
        xcb_image_destroy(m_Canvas);
    }
    xcb_disconnect(m_Connection);
}

void Window::PollEvents() {
    xcb_generic_event_t *event;
    while ((event = xcb_poll_for_event(m_Connection)) != NULL) {
        HandleEvent(event);
    }
} 

void Window::EventLoop() {
    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event(m_Connection)) != NULL) {
        HandleEvent(event);
    }
}

void Window::HandleEvent(xcb_generic_event_t *event) {
    switch (event->response_type & ~0x80) {
        case 0: {
            xcb_generic_error_t *err = (xcb_generic_error_t *)event; 
            std::cout << "err: " << err->error_code << "\n";
            break;
        }
        case XCB_EXPOSE: {
            xcb_expose_event_t *ev = (xcb_expose_event_t *)event;
            xcb_image_put(m_Connection, ev->window, m_GC, m_Canvas, 0, 0, 0);
            xcb_flush(m_Connection);
            break;
        }
        case XCB_CONFIGURE_NOTIFY: {
            xcb_configure_notify_event_t *ev = (xcb_configure_notify_event_t *)event;
            if (m_Canvas->width != ev->width || m_Canvas->height != ev->height) {
                OnResize(ev->width, ev->height); 
            }
            break;
        }
    }
    free(event); 
}

void Window::CreateCanvas() {
    if (m_Canvas != nullptr)
        xcb_image_destroy(m_Canvas);
    uint32_t bytes = m_Width * m_Height * m_Screen->root_depth / 8;
    m_Canvas = xcb_image_create_native(
        m_Connection, 
        m_Width, 
        m_Height, 
        XCB_IMAGE_FORMAT_Z_PIXMAP, 
        m_Screen->root_depth, 
        NULL, 
        bytes, 
        NULL);

    uint32_t red_pixel = 255 << 16;
    for (uint32_t x = 0; x < m_Width; x++) {
        for (uint32_t y = 0; y < m_Height; y++) {
            xcb_image_put_pixel(m_Canvas, x, y, red_pixel); 
        }
    }
}

void Window::OnResize(uint32_t w, uint32_t h) {
    m_Width = w;
    m_Height = h;
    CreateCanvas(); 
}