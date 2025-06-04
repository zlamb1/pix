#ifndef PIX_WINDOW_H
#define PIX_WINDOW_H 1

#include "libbase/pix_type.h"

#include "libwin/pix_bitmap.h"

typedef enum
pxWindowTag
{
    PIX_WINDOW_TAG_POS        = 0x1,
    PIX_WINDOW_TAG_SIZE       = 0x2,
    PIX_WINDOW_TAG_TITLE      = 0x4,
    PIX_WINDOW_TAG_GL_CONTEXT = 0x8
} pxWindowTag;

typedef struct
pxWindowTags
{
    pxWindowTag tags;
    pxInt x, y; 
    pxUnsigned width, height;
    pxChar *title; 
} pxWindowTags;

typedef struct 
pxWindow pxWindow;

typedef struct
pxWindowPos
{
    pxInt x, y; 
} pxWindowPos;

typedef struct 
pxWindowSize
{
    pxUnsigned width, height; 
} pxWindowSize;

typedef void (*pxWindowExposeCallback)(pxWindow *);
typedef void (*pxWindowPosCallback)(pxWindow *, pxInt, pxInt); 
typedef void (*pxWindowSizeCallback)(pxWindow *, pxUnsigned, pxUnsigned);
typedef void (*pxWindowCloseCallback)(pxWindow *); 

pxResult
pxInitWindow(const pxWindowTags *tags, pxWindow **window);

pxBool 
pxWindowShouldClose(pxWindow *window);

void *
pxWindowGetUserPointer(pxWindow *window); 

void 
pxWindowSetUserPointer(pxWindow *window, void *userPointer); 

void
pxWindowSetExposeCallback(pxWindow *window, pxWindowExposeCallback callback);

void 
pxWindowSetPosCallback(pxWindow *window, pxWindowPosCallback callback); 

void 
pxWindowSetSizeCallback(pxWindow *window, pxWindowSizeCallback callback);

void 
pxWindowSetCloseCallback(pxWindow *window, pxWindowCloseCallback callback); 

void
pxWindowQueueEvents(pxWindow *window, pxBool block);

void
pxWindowDispatchEvents(pxWindow *window);

void
pxWindowPollEvents(pxWindow *window, pxBool block); 

pxWindowPos
pxWindowGetPos(pxWindow *window);

pxResult 
pxWindowSetPos(pxWindow *window, pxInt x, pxInt y); 

pxWindowSize
pxWindowGetSize(pxWindow *window); 

pxResult 
pxWindowSetSize(pxWindow *window, pxUnsigned width, pxUnsigned height); 

const pxChar *
pxWindowGetTitle(pxWindow *window); 

pxResult
pxWindowSetTitle(pxWindow *window, const pxChar *title); 

pxResult 
pxWindowSetTitleWithLength(pxWindow *window, const pxChar *title, pxUnsigned len); 

pxResult
pxWindowInvalidateFrame(pxWindow *window); 

pxResult
pxWindowSwapBuffers(pxWindow *window);

pxResult
pxInitFrameBitmap(pxWindow *window, void *base, pxBitmap **bitmap);

pxResult
pxInitBitmap(pxWindow *window, pxUnsigned width, pxUnsigned height, void *base, pxBitmap **bitmap); 

pxResult 
pxBlitBitmap(pxWindow *window, pxBitmap *bitmap); 

pxResult
pxDestroyBitmap(pxWindow *window, pxBitmap *bitmap); 

pxResult
pxCloseWindow(pxWindow *window);

pxResult
pxDestroyWindow(pxWindow *window);  

#endif
