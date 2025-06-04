#ifdef PIX_PLATFORM_WIN32

#include <process.h>
#include <windows.h>

#include "libbase/pix_assert.h"
#include "libbase/pix_error.h"

#include "libthread/pix_thread.h"
#include "libthread/pix_thread_local.h"

typedef struct
pxThreadAffinity
{
    DWORD id; 
} pxThreadAffinity;

typedef struct
pxSemaphore
{
    HANDLE hnd;
} pxSemaphore; 

typedef struct
pxMutex
{
    HANDLE hnd; 
} pxMutex;

typedef struct
pxThread
{
    pxThreadProc proc;
    pxMutex *mutex; 
    pxBool detatched, exited;
    HANDLE hnd;
    void *arg, *retVal; 
} pxThread;

static __pxThreadLocal DWORD threadAffinityInit = 0;
static __pxThreadLocal pxThreadAffinity threadAffinity; 
static __pxThreadLocal pxThread *threadStorage = NULL; 

pxThreadAffinity *
pxGetCurrentThreadAffinity(void)
{
    // FIXME: think about C constructors?
    if (!threadAffinityInit) {
        threadAffinity.id = GetCurrentThreadId(); 
        threadAffinityInit = 1; 
    }
    return &threadAffinity;
}

static unsigned int 
_pxThreadProc(void *p)
{
    pxThread *thread;
    PIX_ASSERT(p != NULL);
    thread = (pxThread *) p; 
    PIX_ASSERT(thread->proc != NULL);
    pxExitThread(thread->proc(thread->arg));
    return 0; 
}

pxThread *
pxCreateThread(pxThreadProc proc, void *arg)
{
    pxThread *thread = malloc(sizeof(pxThread));
    if ( thread == NULL )
        return NULL; 
    thread->mutex = pxCreateMutex();
    if ( thread->mutex == NULL )
        return NULL; 
    thread->detatched = PIX_FALSE;
    thread->exited = PIX_FALSE; 
    thread->proc = proc;
    thread->arg = arg;
    thread->retVal = NULL;
    /* create suspended to avoid any races with HANDLE access */
    // FIXME: use mutex instead
    thread->hnd = (HANDLE) (uintptr_t) _beginthreadex(
        NULL, 0,
        _pxThreadProc, thread,
        CREATE_SUSPENDED, NULL
    );
    if ( thread->hnd == NULL || ResumeThread(thread->hnd) == (DWORD) -1 )
        goto pxThreadCreateFail;
    return thread;
    pxThreadCreateFail:
    pxDestroyMutex(thread->mutex); 
    free(thread);
    return NULL;
}

pxResult
pxJoinThread(pxThread *thread, void **ret)
{
    PIX_ASSERT(thread != NULL); 
    WaitForSingleObject(thread->hnd, INFINITE);
    if (ret != NULL)
        *ret = thread->retVal;
    CloseHandle(thread->hnd);
    pxDestroyMutex(thread->mutex);
    free(thread);
    return PIX_SUCCESS;
}

pxResult
pxDetatchThread(pxThread *thread)
{
    pxResult result;
    PIX_ASSERT(thread != NULL);
    if ( ( result = pxLockMutex(thread->mutex) ) != PIX_SUCCESS )
        return result;
    PIX_ASSERT(thread->detatched == PIX_FALSE);
    thread->detatched = PIX_TRUE; 
    CloseHandle(thread->hnd); 
    thread->hnd = NULL; 
    if ( thread->exited ) {
        pxReleaseMutex(thread->mutex); 
        pxDestroyMutex(thread->mutex); 
        free(thread);
    }
    return PIX_SUCCESS;
}

pxResult
pxExitThread(void *ret)
{
    if (threadStorage != NULL && pxLockMutex(threadStorage->mutex) == PIX_SUCCESS ) {
        PIX_ASSERT(threadStorage->exited == PIX_FALSE);
        threadStorage->retVal = ret;
        threadStorage->exited = PIX_TRUE;
        if (threadStorage->detatched == PIX_TRUE) {
            /* cleanup detatched thread */
            pxReleaseMutex(threadStorage->mutex); 
            pxDestroyMutex(threadStorage->mutex); 
            free(threadStorage);
        } else {
            pxReleaseMutex(threadStorage->mutex);
        }
    }
    ExitThread(0);
}

pxSemaphore *
pxCreateSemaphore(unsigned initialCount)
{
    pxSemaphore *semaphore;
    PIX_ASSERT(initialCount <= LONG_MAX);
    semaphore = malloc(sizeof(pxSemaphore));
    if ( semaphore == NULL )
        return NULL;
    semaphore->hnd = CreateSemaphore(NULL, initialCount, LONG_MAX, NULL);
    return semaphore;
}

pxResult
pxLockSemaphore(pxSemaphore *semaphore)
{
    DWORD lock;
    PIX_ASSERT(semaphore != NULL);
    lock = WaitForSingleObject(semaphore->hnd, INFINITE);
    if ( lock == WAIT_ABANDONED || lock == WAIT_OBJECT_0 )
        return PIX_SUCCESS;
    return PIX_ERR_LOCK;
}

pxResult
pxTryLockSemaphore(pxSemaphore *semaphore)
{
    DWORD lock;
    PIX_ASSERT(semaphore != NULL);
    lock = WaitForSingleObject(semaphore->hnd, 0);
    if ( lock == WAIT_ABANDONED || lock == WAIT_OBJECT_0 )
        return PIX_SUCCESS;
    else if ( lock == WAIT_TIMEOUT )
        return PIX_ERR_TIMEOUT; 
    return PIX_ERR_LOCK;
}

pxResult
pxReleaseSemaphore(pxSemaphore *semaphore)
{
    LONG prevCount;
    PIX_ASSERT(semaphore != NULL);
    if ( ReleaseSemaphore(semaphore->hnd, 1, &prevCount) == 0)
        return PIX_ERR_RELEASE; 
    return PIX_SUCCESS;
}

pxResult
pxDestroySemaphore(pxSemaphore *semaphore)
{
    PIX_ASSERT(semaphore != NULL);
    CloseHandle(semaphore->hnd); 
    free(semaphore);
    return PIX_SUCCESS;
}

pxMutex *
pxCreateMutex(void)
{
    pxMutex *mutex = malloc(sizeof(pxMutex));
    if (mutex == NULL)
        return NULL;
    mutex->hnd = CreateMutex(NULL, FALSE, NULL); 
    return mutex;
}

pxResult
pxLockMutex(pxMutex *mutex)
{
    DWORD lock;
    PIX_ASSERT(mutex != NULL);
    lock = WaitForSingleObject(mutex->hnd, INFINITE);
    if ( lock == WAIT_ABANDONED || lock == WAIT_OBJECT_0 )
        return PIX_SUCCESS;
    return PIX_ERR_LOCK;
}

pxResult
pxTryLockMutex(pxMutex *mutex)
{
    DWORD lock;
    PIX_ASSERT(mutex != NULL);
    lock = WaitForSingleObject(mutex->hnd, 0);
    if ( lock == WAIT_ABANDONED || lock == WAIT_OBJECT_0 )
        return PIX_SUCCESS;
    else if ( lock == WAIT_TIMEOUT )
        return PIX_ERR_TIMEOUT; 
    return PIX_ERR_LOCK;
}

pxResult
pxReleaseMutex(pxMutex *mutex)
{
    PIX_ASSERT(mutex != NULL);
    if ( ReleaseMutex(mutex->hnd) == 0 )
        return PIX_ERR_RELEASE;
    return PIX_SUCCESS;
}

pxResult
pxDestroyMutex(pxMutex *mutex)
{
    PIX_ASSERT(mutex != NULL);
    CloseHandle(mutex->hnd);
    free(mutex);
    return PIX_SUCCESS;
}

#endif
