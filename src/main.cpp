#include <stdlib.h>
#include <xcb/xcb.h>

#define XSTR(x) STR(x)
#define STR(x) #x

int main(void) {
    xcb_connection_t *connection = xcb_connect (NULL, NULL);

    const xcb_setup_t      *setup  = xcb_get_setup (connection);
    xcb_screen_iterator_t   iter   = xcb_setup_roots_iterator (setup);
    xcb_screen_t           *screen = iter.data;

    xcb_window_t window = xcb_generate_id (connection);

    uint32_t     mask      = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t     values[2] = {screen->white_pixel, XCB_EVENT_MASK_EXPOSURE };

    xcb_create_window(connection,    
                        0,                             /* depth               */
                        window,                        
                        screen->root,                  /* parent window       */
                        0, 0,                          /* x, y                */
                        150, 150,                      /* width, height       */
                        10,                            /* border_width        */
                        XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                        screen->root_visual,           /* visual              */
                        mask, values );                /* masks */

    /* Map the window on the screen */
    xcb_map_window(connection, window);

     /* Make sure commands are sent before we pause so that the window gets shown */
    xcb_flush(connection);

    xcb_generic_event_t *event;
    while ( (event = xcb_wait_for_event (connection)) ) {
        free(event);
    }

    xcb_disconnect(connection);

    return 0; 
}