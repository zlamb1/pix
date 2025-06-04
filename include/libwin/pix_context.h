#ifndef PIX_CONTEXT_H
#define PIX_CONTEXT_H 1

#include "pix_window.h"

pxResult
pxInitContext(void);

pxResult
pxCreateContext(pxWindow *window);

pxResult
pxMakeContextCurrent(pxWindow *window); 

#endif
