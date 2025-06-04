#ifndef PIX_WINDOW_INTERNAL_H 
#define PIX_WINDOW_INTERNAL_H 1

#include <stdatomic.h>

#include "libbase/pix_error.h"

#include "libthread/pix_thread.h"

#include "libwin/pix_window.h"

#ifndef PIX_WINDOW_EVENT_QUEUE_SIZE
#define PIX_WINDOW_EVENT_QUEUE_SIZE 256
#endif

typedef struct 
pxEventQueue
{
    pxSemaphore *semaphore;
    pxMutex *mutex;
    pxWindowEvent *queue, *read, *write;
} pxEventQueue;

typedef struct 
pxWindow
{
    pxBool shouldClose; 
    pxInt x, y;
    pxUnsigned width, height; 
    pxChar *title; 
    void *userPointer; 
    void (*exposeCallback)(struct pxWindow *); 
    void (*posCallback)(struct pxWindow *, pxInt, pxInt); 
    void (*sizeCallback)(struct pxWindow *, pxUnsigned, pxUnsigned);
    void (*closeCallback)(struct pxWindow *); 
    pxBool isQueuing; 
    _Atomic(unsigned int) eventMask; 
    pxEventQueue eventQueues[PIX_WINDOW_EVENT_TYPE_MAX]; 
} pxWindow;

static pxWindowEventTypeMask _pxEventMasks[PIX_WINDOW_EVENT_TYPE_MAX] = {
    PIX_WINDOW_EVENT_EXPOSE_MASK,
    PIX_WINDOW_EVENT_POS_MASK,
    PIX_WINDOW_EVENT_SIZE_MASK,
    PIX_WINDOW_EVENT_CLOSE_MASK
};

pxResult
_pxInitBaseWindow(const pxWindowTags *tags, pxWindow *window);

pxResult
_pxEnqueueEvent(pxWindow *window, pxWindowEvent event, pxBool overwrite);

pxResult
_pxDequeueEvent(pxWindow *window, pxWindowEventType eventType, pxWindowEvent *out);

pxResult
_pxDestroyBaseWindow(pxWindow *window);

#endif
