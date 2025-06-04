#ifdef PIX_PLATFORM_WIN32

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

#include "libwin/pix_context.h"

#include "pix_bitmap_win32.h"
#include "pix_thunk.h"
#include "pix_window_win32.h"

static const pxWindowTags defWindowTags = {
    .tags = 0
};

static LRESULT CALLBACK 
pxWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void *p);

pxResult
pxInitWindow(const pxWindowTags *tags, pxWindow **out)
{
    /* FIXME: handle creation of multiple windows */
    const wchar_t CLASS_NAME[]  = L"PIX";
    pxWindowWin32 *window = malloc(sizeof(pxWindowWin32)); 
    pxBool classInit = PIX_FALSE;
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = {0};
    PFNWNDPROC wndProcThunk = NULL;
    pxResult result = PIX_ERR_ALLOC;
    PIX_ASSERT(out != NULL);
    if ( tags == NULL )
        tags = &defWindowTags;  
    if ( window == NULL )
        goto pxCleanup;
    memset(window, 0, sizeof(pxWindowWin32));
    if ( ( result = _pxInitBaseWindow(tags, &window->base) ) != PIX_SUCCESS )
        goto pxCleanup;
    if ( ( wndProcThunk = pxCreateWndProcThunk(pxWindowProc, window) ) == NULL )
        goto pxCleanup;
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = wndProcThunk;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW); 
    wc.hbrBackground = NULL; 
    wc.lpszMenuName  = NULL; 
    wc.lpszClassName = CLASS_NAME;
    if ( RegisterClass(&wc) != 0 )
       classInit = PIX_TRUE;
    window->win32HWND = CreateWindowEx(
        0,
        CLASS_NAME,
        L"",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if ( window->win32HWND == NULL )
        goto pxCleanup;
    window->win32HDC = GetDC(window->win32HWND);
    if ( tags->tags & PIX_WINDOW_TAG_GL_CONTEXT ) {
        if ( ( result = pxCreateContext(&window->base) ) != PIX_SUCCESS )
            goto pxCleanup;
    }
    if ( tags->tags & PIX_WINDOW_TAG_EVENT_MASK ) {

    }
    ShowWindow(window->win32HWND, SW_NORMAL);
    *out = &window->base;
    return PIX_SUCCESS;
    pxCleanup:
    if ( window != NULL ) {
        _pxDestroyBaseWindow(&window->base);
        if ( window->win32HWND != NULL )
            DestroyWindow(window->win32HWND);
        if ( classInit )
            UnregisterClass(CLASS_NAME, hInstance); 
        if ( wndProcThunk != NULL )
            pxFreeWndProcThunk(wndProcThunk);
        free(window); 
    }
    return result;
}

pxResult
pxCloseWindow(pxWindow *base)
{
    PIX_GET_WIN32_INSTANCE(base);
    if ( PostMessage(window->win32HWND, WM_CLOSE, (WPARAM) NULL, (LPARAM) NULL) == 0 )
        return PIX_ERR_PLATFORM; 
    return PIX_SUCCESS;
}

void
pxWindowQueueEvents(struct pxWindow *base, pxBool block)
{
    MSG msg = {0};
    PIX_GET_WIN32_INSTANCE(base);
    window->base.isQueuing = PIX_TRUE; 
    if ( block && GetMessage(&msg, window->win32HWND, 0, 0) != 0 ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    while ( PeekMessage(&msg, window->win32HWND, 0, 0, PM_REMOVE) != 0 ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void
pxWindowDispatchEvents(struct pxWindow *base)
{
    pxWindowEvent event;
    PIX_GET_WIN32_INSTANCE(base);
    while ( pxDequeueEvent(&window->base, PIX_WINDOW_EVENT_TYPE_MAX, &event) == PIX_SUCCESS ) {
        switch ( event.eventType ) {
            case PIX_WINDOW_EVENT_TYPE_EXPOSE:
                if (base->exposeCallback != NULL)
                    base->exposeCallback(base);
                break;
            case PIX_WINDOW_EVENT_TYPE_POS:
                if (base->posCallback != NULL)
                    base->posCallback(base, event.positionEvent.x, event.positionEvent.y);
                break;
            case PIX_WINDOW_EVENT_TYPE_SIZE:
                if (base->sizeCallback != NULL)
                    base->sizeCallback(base, event.sizeEvent.width, event.sizeEvent.height);
                break;
            case PIX_WINDOW_EVENT_TYPE_CLOSE:
                if (base->closeCallback != NULL)
                    base->closeCallback(base);
                break;
            default:
                break;
        }
    }
}

void
pxWindowPollEvents(struct pxWindow *base, pxBool block)
{
    MSG msg = {0};
    PIX_GET_WIN32_INSTANCE(base);
    window->base.isQueuing = PIX_FALSE; 
    if ( block && GetMessage(&msg, window->win32HWND, 0, 0) != 0 ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    while ( PeekMessage(&msg, window->win32HWND, 0, 0, PM_REMOVE) ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

pxWindowPos
pxWindowGetPos(pxWindow *base)
{
    PIX_GET_WIN32_INSTANCE(base);
    return (pxWindowPos) {
        window->base.x,
        window->base.y
    };
}

pxResult 
pxWindowSetPos(struct pxWindow *base, pxInt x, pxInt y)
{
    PIX_GET_WIN32_INSTANCE(base);
    if ( SetWindowPos(window->win32HWND, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE) == 0)
        return PIX_ERR_PLATFORM; 
    return PIX_SUCCESS;
}

struct pxWindowSize
pxWindowGetSize(struct pxWindow *base)
{
    PIX_GET_WIN32_INSTANCE(base);
    return (pxWindowSize) {
        window->base.width,
        window->base.height
    };
}

pxResult 
pxWindowSetSize(struct pxWindow *base, pxUnsigned width, pxUnsigned height)
{
    PIX_GET_WIN32_INSTANCE(base);
    if ( SetWindowPos(window->win32HWND, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE) == 0 )
        return PIX_ERR_PLATFORM;
    return PIX_SUCCESS;
}

const pxChar *
pxWindowGetTitle(struct pxWindow *base)
{
    PIX_GET_WIN32_INSTANCE(base);
    return window->base.title;
}

pxResult 
pxWindowSetTitleWithLength(struct pxWindow *base, const pxChar *title, pxUnsigned len)
{
    pxInt size;
    LPWSTR buf;
    PIX_GET_WIN32_INSTANCE(base); 
    size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, title, -1, NULL, 0);
    (void) len; 
    if ( size == 0 )
        return PIX_ERR_PLATFORM;
    buf = malloc(size * sizeof(wchar_t));
    if ( buf == NULL )
        return PIX_ERR_ALLOC;
    if ( MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, title, -1, buf, size) == 0 ) {
        free(buf);
        return PIX_ERR_PLATFORM;
    }
    if ( SetWindowTextW(window->win32HWND, (LPCWSTR) buf) == 0 ) {
        free(buf);
        return PIX_ERR_PLATFORM; 
    }
    free(buf);
    return PIX_SUCCESS;
}

pxResult
pxWindowInvalidateFrame(pxWindow *base)
{
    PIX_GET_WIN32_INSTANCE(base);
    if ( InvalidateRect(window->win32HWND, NULL, 0) == 0 )
        return PIX_ERR_PLATFORM;  
    return PIX_SUCCESS;
}

pxResult
pxWindowSwapBuffers(pxWindow *base)
{
    PIX_GET_WIN32_INSTANCE(base);
    PIX_ASSERT(window->win32HDC != NULL);
    if ( SwapBuffers(window->win32HDC) == FALSE )
        return PIX_ERR_PLATFORM;
    return PIX_SUCCESS;
}

pxResult 
pxDestroyWindow(pxWindow *base)
{
    PIX_GET_WIN32_INSTANCE(base); 
    if ( base->title != NULL )
        free(base->title);
    _pxDestroyBaseWindow(base);
    free(window); 
    return PIX_SUCCESS;
}

LRESULT CALLBACK 
pxWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, void *p)
{
    pxWindowWin32 *window = (pxWindowWin32 *) p; 
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT paint;
            BeginPaint(hWnd, &paint);
            EndPaint(hWnd, &paint);
            if (window->base.isQueuing) {
                pxWindowEvent event = {
                    .eventType = PIX_WINDOW_EVENT_TYPE_EXPOSE,
                };
                _pxEnqueueEvent(&window->base, event, PIX_TRUE); 
            } else if (window->base.exposeCallback != NULL) {
                window->base.exposeCallback(&window->base);
            }
            return 0;
        }
        case WM_SIZE:
            window->base.width = LOWORD(lParam);
            window->base.height = HIWORD(lParam);
            if (window->base.isQueuing) {
                pxWindowEvent event = {
                    .eventType = PIX_WINDOW_EVENT_TYPE_SIZE,
                    .sizeEvent = {
                        .width = window->base.width,
                        .height = window->base.height
                    }
                };
                _pxEnqueueEvent(&window->base, event, PIX_TRUE); 
            } else if (window->base.sizeCallback != NULL) {
                window->base.sizeCallback(&window->base, window->base.width, window->base.height);
            }
            return 0;
        case WM_DESTROY:
            window->base.shouldClose = PIX_TRUE; 
            if (window->base.isQueuing) {
                pxWindowEvent event = {
                    .eventType = PIX_WINDOW_EVENT_TYPE_CLOSE,
                };
               _pxEnqueueEvent(&window->base, event, PIX_TRUE); 
            } else if (window->base.closeCallback != NULL) {
                window->base.closeCallback(&window->base);
            }
            PostQuitMessage(0);
            return 0;
        case WM_SETTEXT: {
            pxInt size = WideCharToMultiByte(
                CP_UTF8, WC_ERR_INVALID_CHARS, 
                (LPCWCH) lParam, -1,
                NULL, 0,
                NULL, NULL
            );
            if ( size != 0 ) {
                pxChar *title = realloc(window->base.title, size); 
                if ( title != NULL ) {
                    WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, 
                        (LPCWCH) lParam, -1,
                        (LPSTR) title, size,
                        NULL, NULL
                    );
                }
            }
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#endif
