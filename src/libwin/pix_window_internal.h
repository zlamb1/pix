#ifndef PIX_WINDOW_INTERNAL_H 
#define PIX_WINDOW_INTERNAL_H 1

#include "libbase/pix_type.h"

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
} pxWindow;

#endif
