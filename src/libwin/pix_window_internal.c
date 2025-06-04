#include "pix_window_internal.h"

pxResult
_pxInitBaseWindow(const pxWindowTags *tags, pxWindow *window)
{
    pxResult result = PIX_ERR_ALLOC;
    PIX_ASSERT(tags != NULL);
    PIX_ASSERT(window != NULL);
    window->shouldClose = PIX_FALSE;
    window->title = NULL;
    window->userPointer = NULL;
    window->exposeCallback = NULL;
    window->posCallback = NULL; 
    window->sizeCallback = NULL;
    window->closeCallback = NULL;
    window->isQueuing = PIX_FALSE;
    window->eventMask = tags->eventMask; 
    for (unsigned i = 0; i < PIX_WINDOW_EVENT_TYPE_MAX; i++) {
        window->eventQueues[i].semaphore = NULL;
        window->eventQueues[i].mutex = NULL;
        window->eventQueues[i].queue = NULL;
        window->eventQueues[i].read = NULL;
        window->eventQueues[i].write = NULL;
    }
    if ( tags->eventMask ) {
        for (unsigned i = 0; i < PIX_WINDOW_EVENT_TYPE_MAX; i++) {
            PIX_ASSERT(_pxEventMasks[i] != 0);
            if ( tags->eventMask & _pxEventMasks[i] ) {
                window->eventQueues[i].semaphore = pxCreateSemaphore(0);
                if ( window->eventQueues[i].semaphore == NULL ) {
                    result = PIX_ERR_ALLOC;
                    goto pxFail;
                }
                window->eventQueues[i].mutex = pxCreateMutex();
                if ( window->eventQueues[i].mutex == NULL ) {
                    result = PIX_ERR_ALLOC;
                    goto pxFail; 
                }
                window->eventQueues[i].queue = malloc(PIX_WINDOW_EVENT_QUEUE_SIZE * sizeof(pxWindowEvent)); 
                if ( window->eventQueues[i].queue == NULL ) {
                    result = PIX_ERR_ALLOC;
                    goto pxFail; 
                }
                window->eventQueues[i].read = window->eventQueues[i].queue;
                window->eventQueues[i].write = window->eventQueues[i].queue;
            }
        }
    }
    return PIX_SUCCESS;
    pxFail:
    for (unsigned i = 0; i < PIX_WINDOW_EVENT_TYPE_MAX; i++) {
        if ( window->eventQueues[i].semaphore != NULL )
            pxDestroySemaphore(window->eventQueues[i].semaphore);
        if ( window->eventQueues[i].mutex != NULL )
            pxDestroyMutex(window->eventQueues[i].mutex);
        if ( window->eventQueues[i].queue != NULL )
            free(window->eventQueues[i].queue); 
        window->eventQueues[i].queue = NULL;
        window->eventQueues[i].read  = NULL;
        window->eventQueues[i].write = NULL;
    }
    return result;
}

pxResult
_pxEnqueueEvent(pxWindow *window, pxWindowEvent event, pxBool overwrite)
{
#define QUEUE_NEXT(P, T) ( ( (P) == (window->eventQueues[(T)].queue + (PIX_WINDOW_EVENT_QUEUE_SIZE - 1)) ) ? window->eventQueues[(T)].queue : (P) + 1 )
    pxResult result;
    pxWindowEvent *next;
    PIX_ASSERT(window != NULL);
    PIX_ASSERT(PIX_WINDOW_EVENT_QUEUE_SIZE > 1); 
    PIX_ASSERT(event.eventType >= 0 && event.eventType < PIX_WINDOW_EVENT_TYPE_MAX);
    if ( ( result = pxLockMutex(window->eventQueues[event.eventType].mutex) ) != PIX_SUCCESS )
        return result;
    next = QUEUE_NEXT(window->eventQueues[event.eventType].write, event.eventType);
    if ( window->eventQueues[event.eventType].read == next ) {
        if ( overwrite ) {
            window->eventQueues[event.eventType].read = QUEUE_NEXT(window->eventQueues[event.eventType].read, event.eventType);
            window->eventQueues[event.eventType].write = QUEUE_NEXT(window->eventQueues[event.eventType].write, event.eventType);
            *window->eventQueues[event.eventType].write = event; 
            pxReleaseMutex(window->eventQueues[event.eventType].mutex);
            return 1;
        }
        pxReleaseMutex(window->eventQueues[event.eventType].mutex);
        return PIX_ERR_BUF_FULL;
    }
    *window->eventQueues[event.eventType].write = event;
    if ( next != window->eventQueues[event.eventType].read )
        window->eventQueues[event.eventType].write = next;
    pxReleaseSemaphore(window->eventQueues[event.eventType].semaphore);
    pxReleaseMutex(window->eventQueues[event.eventType].mutex);
    return PIX_SUCCESS;
}

pxResult
pxDequeueEvent(pxWindow *window, pxWindowEventType eventType, pxWindowEvent *out)
{
    pxResult result;
    PIX_ASSERT(window != NULL);
    PIX_ASSERT(PIX_WINDOW_EVENT_QUEUE_SIZE > 1);
    PIX_ASSERT(eventType >= 0 && eventType <= PIX_WINDOW_EVENT_TYPE_MAX);
    if ( eventType == PIX_WINDOW_EVENT_TYPE_MAX ) {
        for (unsigned i = 0; i < PIX_WINDOW_EVENT_TYPE_MAX; i++) {
            if ( window->eventMask & _pxEventMasks[i] && ( result = pxDequeueEvent(window, i, out) ) == PIX_SUCCESS )
                return PIX_SUCCESS;
        }
        return PIX_ERR_NOT_FOUND;
    }
    if ( ( result = pxTryLockSemaphore(window->eventQueues[eventType].semaphore) ) != PIX_SUCCESS ) {
        if (result == PIX_ERR_TIMEOUT)
            return PIX_ERR_BUF_EMPTY; 
        return result;
    }
    if ( ( result = pxLockMutex(window->eventQueues[eventType].mutex) ) != PIX_SUCCESS) 
        return result;
    if ( window->eventQueues[eventType].queue == NULL ) {
        pxReleaseMutex(window->eventQueues[eventType].mutex);
        return PIX_ERR_INVALID_ARG;
    }
    if ( window->eventQueues[eventType].read == window->eventQueues[eventType].write )
        return PIX_ERR_BUF_EMPTY; 
    *out = *window->eventQueues[eventType].read;
    window->eventQueues[eventType].read = QUEUE_NEXT(window->eventQueues[eventType].read, eventType);
    pxReleaseMutex(window->eventQueues[eventType].mutex); 
    return PIX_SUCCESS;
#undef QUEUE_NEXT
}

pxResult
_pxDestroyBaseWindow(pxWindow *window)
{
    PIX_ASSERT(window != NULL); 
    for (unsigned i = 0; i < PIX_WINDOW_EVENT_TYPE_MAX; i++) {
        if ( window->eventQueues[i].semaphore != NULL )
            pxDestroySemaphore(window->eventQueues[i].semaphore);
        if ( window->eventQueues[i].mutex != NULL )
            pxDestroyMutex(window->eventQueues[i].mutex);
        if ( window->eventQueues[i].queue != NULL )
            free(window->eventQueues[i].queue); 
    }
    return PIX_SUCCESS;
}
