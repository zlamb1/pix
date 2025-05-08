#pragma once

#include <stdint.h>

#include <xcb/xcb.h>
#include <xcb/xcb_image.h>
#include <xcb/xproto.h>

class Window {
    public:
        Window(); 
        ~Window(); 

        void PollEvents(); 
        void EventLoop(); 
        
    protected:
        uint32_t m_Width, m_Height; 

        xcb_connection_t *m_Connection;
        xcb_screen_t *m_Screen;
        xcb_window_t m_Window;
        xcb_image_t *m_Canvas = nullptr; 
        xcb_gcontext_t m_GC; 

        void HandleEvent(xcb_generic_event_t *event); 
        void CreateCanvas(); 
        void OnResize(uint32_t w, uint32_t h); 
};