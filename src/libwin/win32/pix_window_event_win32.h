#ifndef PIX_WINDOW_EVENT_WIN32_H
#define PIX_WINDOW_EVENT_WIN32_H

#include "libbase/pix_pack.h"

#include "libthread/pix_thread.h"

typedef enum
pxWindowEventType
{
    PIX_WINDOW_EVENT_EXPOSE,
    PIX_WINDOW_EVENT_POS,
    PIX_WINDOW_EVENT_SIZE,
    PIX_WINDOW_EVENT_CLOSE
} pxWindowEventType;

typedef struct
pxWindowPositionEvent
{
    int x;
    int y;
} pxWindowPositionEvent;

typedef struct
pxWindowSizeEvent
{
    unsigned width;
    unsigned height;
} pxWindowSizeEvent;

typedef union 
pxWindowEventU
{
    pxWindowPositionEvent positionEvent;
    pxWindowSizeEvent sizeEvent; 
} pxWindowEventU;

typedef struct pxWindowEvent
{
    pxWindowEventType eventType;
    pxWindowEventU event; 
} pxWindowEvent;

#endif
