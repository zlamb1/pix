#ifndef PIX_H
#define PIX_H 1

#include "libbase/pix_assert.h"
#include "libbase/pix_error.h"

#include "libgl/pix_gl.h"

#include "libthread/pix_thread.h"

#include "libtime/pix_time.h"

#include "libwin/pix_context.h"

static pxResult 
pxInit()
{
    pxResult result;
    if ( ( result = pxInitTime() ) != PIX_SUCCESS )
        return result;
    if ( ( result = pxInitContext() ) != PIX_SUCCESS )
        return result;
    if ( ( result = pxInitGL() ) != PIX_SUCCESS )
        return result;
    return PIX_SUCCESS; 
}

#endif
