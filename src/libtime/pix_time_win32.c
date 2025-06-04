#ifdef PIX_PLATFORM_WIN32

#include <windows.h>

#include "libbase/pix_error.h"

#include "libtime/pix_time.h"

static double __pxFreq = 1; 

pxResult
pxInitTime(void)
{
    LARGE_INTEGER i;
    QueryPerformanceFrequency(&i);
    __pxFreq = (double) i.QuadPart;
    return PIX_SUCCESS;
}

double 
pxGetTime(void)
{
    LARGE_INTEGER i;
    QueryPerformanceCounter(&i);
    return ( (double) i.QuadPart ) / __pxFreq;  
}

#endif
