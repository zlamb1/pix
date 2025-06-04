#ifndef PIX_THREAD_H
#define PIX_THREAD_H 1

#include "libbase/pix_error.h"

typedef void *(*pxThreadProc)(void *);

typedef struct
pxThreadAffinity pxThreadAffinity;

typedef struct 
pxThread pxThread;

typedef struct
pxSemaphore pxSemaphore;

typedef struct
pxMutex pxMutex;

pxThreadAffinity *
pxGetCurrentThreadAffinity(void); 

pxThread *
pxCreateThread(pxThreadProc proc, void *arg);

pxResult 
pxJoinThread(pxThread *thread, void **ret);

pxResult
pxDetatchThread(pxThread *thread); 

pxResult
pxExitThread(void *ret); 

pxSemaphore *
pxCreateSemaphore(unsigned initialCount); 

pxResult
pxLockSemaphore(pxSemaphore *semaphore);

pxResult
pxTryLockSemaphore(pxSemaphore *semaphore); 

pxResult
pxReleaseSemaphore(pxSemaphore *semaphore);

pxResult
pxDestroySemaphore(pxSemaphore *semaphore);

pxMutex *
pxCreateMutex(void);

pxResult
pxLockMutex(pxMutex *mutex);

pxResult
pxTryLockMutex(pxMutex *mutex);

pxResult
pxReleaseMutex(pxMutex *mutex);

pxResult
pxDestroyMutex(pxMutex *mutex);

#endif
