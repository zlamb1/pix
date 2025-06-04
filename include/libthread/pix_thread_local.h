#ifndef PIX_THREAD_LOCAL_H
#define PIX_THREAD_LOCAL_H 1

#ifdef __GNUC__
#define __pxThreadLocal __thread
#endif

#ifdef _MSC_VER
#define __pxThreadLocal __declspec(thread)
#endif

#endif
