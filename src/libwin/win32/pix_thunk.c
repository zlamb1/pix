#ifdef PIX_PLATFORM_WIN32

#include <windows.h>

#include "libbase/pix_array.h"
#include "libbase/pix_assert.h"
#include "libbase/pix_error.h"
#include "libbase/pix_pack.h"

#include "pix_thunk.h"

#define pxWriteThunk pxWriteThunk_x86

PIX_PACK(
typedef struct 
pxThunkHeader
{
    void *head; 
    struct pxThunkHeader *next;
}) pxThunkHeader;

typedef struct 
pxThunkStorage
{
    void *base;
    pxThunkHeader *freelist;
    size_t cap, size, bytes; 
    struct pxThunkStorage *next; 
} pxThunkStorage;

typedef struct
pxWinProcAssembly
{
    unsigned char *buf;
    size_t size, cap;  
    unsigned procAddrIdx, pointerAddrIdx; 
} pxWinProcAssembly;

static pxThunkStorage *thunkStorage = NULL;
static pxWinProcAssembly *winProcAssembly = NULL;  

static inline int
pxWriteThunk_x86(pxWinProcAssembly *procAssembly)
{
#define ADD_BYTE(VAL) PIX_ARRAY_APPEND((VAL), procAssembly->buf, 1, procAssembly->size, procAssembly->cap)
    size_t pow2Size;
    PIX_ASSERT(procAssembly != NULL);
    // MOV rax, up
    ADD_BYTE(0x48);
    ADD_BYTE(0xB8);
    procAssembly->pointerAddrIdx = procAssembly->size; 
    for (unsigned i = 0; i < 8; i++)
        ADD_BYTE(0x00); 
    // PUSH rax
    ADD_BYTE(0x50);
    // SUB rsp, 0x20
    ADD_BYTE(0x48);
    ADD_BYTE(0x83); 
    ADD_BYTE(0xEC);
    ADD_BYTE(0x20); 
    // MOV rax, proc
    ADD_BYTE(0x48);
    ADD_BYTE(0xB8);
    procAssembly->procAddrIdx = procAssembly->size; 
    for (unsigned i = 0; i < 8; i++)
        ADD_BYTE(0x00);
    // CALL rax
    ADD_BYTE(0xFF);
    ADD_BYTE(0xD0);
    // ADD rsp, 0x28
    ADD_BYTE(0x48);
    ADD_BYTE(0x83);
    ADD_BYTE(0xC4);
    ADD_BYTE(0x28);
    // RET
    ADD_BYTE(0xC3);
    // pad for header
    pow2Size = procAssembly->size + sizeof(pxThunkHeader);
    pow2Size--;
    pow2Size |= pow2Size >> 1;
    pow2Size |= pow2Size >> 2;
    pow2Size |= pow2Size >> 4;
    pow2Size |= pow2Size >> 8;
    pow2Size |= pow2Size >> 16;
    pow2Size++;
    for (size_t i = 0; i < pow2Size - procAssembly->size; i++)
        ADD_BYTE(0x00); 
    procAssembly->size = pow2Size;
    return 0;
    pxAllocFail:
    return PIX_ERR_ALLOC; 
#undef ADD_BYTE
}

static DWORD
pxGetPageSize(void)
{
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo); 
    return systemInfo.dwPageSize;
}

static pxResult
pxCreateThunkStorage(pxThunkStorage *storage, DWORD pageSize)
{
    pxThunkHeader *header;
    size_t bytes = pageSize, n;
    DWORD flProtect;
    PIX_ASSERT(storage != NULL); 
    PIX_ASSERT(pageSize > 0); 
    memset(storage, 0, sizeof(pxThunkStorage));
    while (bytes < winProcAssembly->size)
        bytes += pageSize; 
    storage->base = VirtualAlloc(NULL, bytes, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (storage->base == NULL)
        return PIX_ERR_ALLOC;
    n = bytes / winProcAssembly->size;
    storage->freelist = NULL; 
    storage->cap = n;
    storage->size = n;
    storage->bytes = bytes; 
    storage->next = NULL;
    header = storage->base;
    while (n > 0) {
        header->head = storage->base;
        header->next = storage->freelist;
        memcpy(header + 1, winProcAssembly->buf, winProcAssembly->size - sizeof(pxThunkHeader));
        storage->freelist = header;
        header = (pxThunkHeader *) (( (char *) storage->freelist ) + winProcAssembly->size);
        n--; 
    }
    if (!VirtualProtect(storage->base, bytes, PAGE_EXECUTE_READ, &flProtect)) {
        VirtualFree(storage->base, 0, MEM_RELEASE); 
        memset(storage, 0, sizeof(pxThunkStorage));
        return PIX_ERR_ALLOC; 
    }
    return PIX_SUCCESS; 
}

PFNWNDPROC
pxCreateWndProcThunk(lpfnWndProc proc, void *up)
{
    DWORD pageSize; 
    pxThunkStorage *currentStorage;
    int allocated = 0; 
    if (winProcAssembly == NULL) {
        winProcAssembly = malloc(sizeof(pxWinProcAssembly)); 
        if (winProcAssembly == NULL)
            return NULL;
        memset(winProcAssembly, 0, sizeof(pxWinProcAssembly)); 
        winProcAssembly->buf = NULL; 
        if ( pxWriteThunk(winProcAssembly) ) {
            free(winProcAssembly->buf);
            free(winProcAssembly); 
            return NULL;
        }
    }
    pageSize = pxGetPageSize(); 
    if (thunkStorage == NULL) {
        thunkStorage = malloc(sizeof(pxThunkStorage));
        if (thunkStorage == NULL)
            return NULL; 
        if (pxCreateThunkStorage(thunkStorage, pageSize)) {
            free(thunkStorage); 
            return NULL; 
        }
    }
    currentStorage = thunkStorage;
    while (1) {
        if (currentStorage->freelist != NULL) {
            unsigned char *fp;
            pxThunkHeader *header;
            DWORD flProtect;
            uintptr_t pointerAddr = (uintptr_t) up, procAddr = (uintptr_t) proc; 
            if (!VirtualProtect(currentStorage->base, currentStorage->bytes, PAGE_EXECUTE_READWRITE, &flProtect))
                return NULL; 
            header = currentStorage->freelist;
            currentStorage->size--;
            currentStorage->freelist = header->next;
            fp = (unsigned char *) (header + 1);
            for (unsigned i = 0; i < 8; i++)
                *(fp + winProcAssembly->pointerAddrIdx + i) = ( (pointerAddr >> i * 8) & 0xFF );
            for (unsigned i = 0; i < 8; i++)
                *(fp + winProcAssembly->procAddrIdx + i) = ( (procAddr >> i * 8) & 0xFF );
            if (!VirtualProtect(currentStorage->base, currentStorage->bytes, PAGE_EXECUTE_READ, &flProtect))
                return NULL; 
            FlushInstructionCache(GetCurrentProcess(), currentStorage->base, currentStorage->bytes); 
            return (PFNWNDPROC) __builtin_assume_aligned(fp, 4); 
        } else {
            if (currentStorage->next == NULL) {
                if (allocated)
                    return NULL; 
                currentStorage->next = malloc(sizeof(pxThunkStorage));
                if (currentStorage->next == NULL)
                    return NULL; 
                if (pxCreateThunkStorage(currentStorage->next, pageSize)) {
                    free(currentStorage->next); 
                    return NULL; 
                }
                allocated = 1; 
            }
            currentStorage = currentStorage->next;
        }
    }
}

int 
pxFreeWndProcThunk(PFNWNDPROC thunk)
{
    char *p = (char *) thunk;
    pxThunkHeader *header = (pxThunkHeader *) (p - sizeof(pxThunkHeader));
    pxThunkStorage *prev = NULL, *storage = thunkStorage;
    while ( storage != NULL ) {
        if (header->head == storage->base) {
            header->next = storage->freelist; 
            storage->freelist = header;
            storage->size++;
            if (storage->size == storage->cap && prev != NULL) {
                // retire storage; but leave at least one thunk storage struct allocated
                prev->next = storage->next;
                if (!VirtualFree(storage->base, 0, MEM_RELEASE)) {
                    // avoid leaking the chunk
                    prev->next = storage;
                    return PIX_ERR_ALLOC;
                }
                free(storage);
            }
            return 0;
        }
        prev = storage; 
        storage = storage->next;
    }
    return PIX_ERR_NOT_FOUND;
}

#endif
