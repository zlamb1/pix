#ifndef PIX_THUNK_H
#define PIX_THUNK_H 1

#include <windows.h>

/*
* Generates thunk assembly for use in lpfnWndProc.  
*/

typedef LRESULT (CALLBACK *lpfnWndProc)(HWND, UINT, WPARAM, LPARAM, void *);
typedef LRESULT (CALLBACK *PFNWNDPROC)(HWND, UINT, WPARAM, LPARAM);

PFNWNDPROC
pxCreateWndProcThunk(lpfnWndProc proc, void *up); 

int 
pxFreeWndProcThunk(PFNWNDPROC thunk);

#endif
