#ifndef PIX_WINDOW_EVENT_H
#define PIX_WINDOW_EVENT_H 1

typedef enum
pxWindowEventType
{
    PIX_WINDOW_EVENT_TYPE_EXPOSE,
    PIX_WINDOW_EVENT_TYPE_POS,
    PIX_WINDOW_EVENT_TYPE_SIZE,
    PIX_WINDOW_EVENT_TYPE_CLOSE,
    PIX_WINDOW_EVENT_TYPE_MAX
} pxWindowEventType;

typedef enum
pxWindowEventTypeMask
{
    PIX_WINDOW_EVENT_EXPOSE_MASK = 0x01,
    PIX_WINDOW_EVENT_POS_MASK    = 0x02,
    PIX_WINDOW_EVENT_SIZE_MASK   = 0x04,
    PIX_WINDOW_EVENT_CLOSE_MASK  = 0x08
} pxWindowEventTypeMask;

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

typedef struct pxWindowEvent
{
    pxWindowEventType eventType;
    union {
        pxWindowPositionEvent positionEvent;
        pxWindowSizeEvent sizeEvent; 
    };
} pxWindowEvent;

typedef struct pxWindow pxWindow;

pxUnsigned
pxWindowGetEventMask(pxWindow *window);

void
pxWindowEventTypeMaskAll(pxWindow *window);

void
pxWindowEventTypeMaskNone(pxWindow *window);

void
pxWindowSetEventTypeMask(pxWindow *window, pxWindowEventTypeMask eventMask);

void
pxWindowMaskEventType(pxWindow *window, pxWindowEventType eventType, pxBool masked);

pxResult
pxDequeueEvent(pxWindow *window, pxWindowEventType eventType, pxWindowEvent *out);

#endif
