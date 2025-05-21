#include "libwin/pix_bitmap.h"
#ifndef pxWindow_H
#define pxWindow_H 1

struct 
pxWindow;

struct
pxWindowPos
{
    int x, y; 
};

struct 
pxWindowSize
{
    unsigned int width, height; 
};

typedef void (*pxWindowExposeCallback)(struct pxWindow *);
typedef void (*pxWindowPosCallback)(struct pxWindow *, int, int); 
typedef void (*pxWindowSizeCallback)(struct pxWindow *, unsigned int, unsigned int);
typedef void (*pxWindowCloseCallback)(struct pxWindow *); 

struct pxWindow *
pxWindowInit(void);

int 
pxWindowShouldClose(struct pxWindow *win);

void *
pxWindowGetUserPointer(struct pxWindow *win); 

void 
pxWindowSetUserPointer(struct pxWindow *win, void *user_pointer); 

void
pxWindowSetExposeCallback(struct pxWindow *win, pxWindowExposeCallback callback);

void 
pxWindowSetPosCallback(struct pxWindow *win, pxWindowPosCallback callback); 

void 
pxWindowSetSizeCallback(struct pxWindow *win, pxWindowSizeCallback callback);

void 
pxWindowSetCloseCallback(struct pxWindow *win, pxWindowCloseCallback callback); 

/**
 * process available events then return control
 */

void
pxWindowPollEvents(struct pxWindow *win); 

/**
 * process and wait for events (does not return control until window closes except for callbacks)
 */

void 
pxWindowWaitEvents(struct pxWindow *win);

struct pxWindowPos
pxWindowGetPos(struct pxWindow *win);

void 
pxWindowSetPos(struct pxWindow *win, int x, int y); 

struct pxWindowSize
pxWindowGetSize(struct pxWindow *win); 

void 
pxWindowSetSize(struct pxWindow *win, unsigned int width, unsigned int height); 

const char *
pxWindowGetTitle(struct pxWindow *win); 

void
pxWindowSetTitle(struct pxWindow *win, const char *title); 

void 
pxWindowSetTitleWithLength(struct pxWindow *win, const char *title, unsigned int len); 

/**
 * returns NULL if bitmap is not supported by window or on error
 */

struct pxBitmap *
pxWindowGetBitmap(struct pxWindow *win, void *base);

struct pxBitmap *
pxWindowGetSizedBitmap(struct pxWindow *win, unsigned width, unsigned height, void *base); 

/**
 * NO_OP if not supported by window
 */

void 
pxWindowBlitBitmap(struct pxWindow *win, struct pxBitmap *bitmap); 

void 
pxDestroyWindow(struct pxWindow *win);  

#endif
