#ifndef PIX_WINDOW_INTERNAL_H 
#define PIX_WINDOW_INTERNAL_H 1

struct 
pxWindow
{
    int shouldClose; 

    void *userPointer; 
    
    void (*exposeCallback)(struct pxWindow *); 
    void (*posCallback)(struct pxWindow *, int, int); 
    void (*sizeCallback)(struct pxWindow *, unsigned int, unsigned int);
    void (*closeCallback)(struct pxWindow *); 
};

#endif
