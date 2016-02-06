#include "window.h"

bool w_open;
int scr, depth, fd;
int _win_h, _win_w;

Display *dpy;
Window rootwin;
Window win;

Colormap cmap;
GC gc;
XEvent ev;

void WCreate(int win_h, int win_w)
{
    _win_w = win_w;
    _win_h = win_h;

    // Try to connect to XServer
    if (!(dpy=XOpenDisplay(NULL))) {
        fprintf(stderr, "ERROR: Could not open display\n");
        exit(1);
    }

    scr = DefaultScreen(dpy);

    // Create Window
    rootwin = RootWindow(dpy, scr);
    win = XCreateSimpleWindow(dpy, rootwin, 1, 1, win_w, win_h, 0,
            BlackPixel(dpy, scr), BlackPixel(dpy, scr));

    cmap = DefaultColormap(dpy, scr);
    gc = XCreateGC(dpy, win, 0, 0);
    XSetForeground(dpy, gc, WhitePixel(dpy, scr));
    XSetBackground(dpy, gc, 0);
    XSelectInput(dpy, win, ExposureMask | ButtonPressMask);


    // Connect the window to the XServer
    XStoreName(dpy, win, "floating");
    XMapWindow(dpy, win);
    XFlush(dpy);

    w_open = true;
}

void WHandleEvents() 
{
    XCheckMaskEvent(dpy, ExposureMask|ButtonPressMask, &ev);

    switch (ev.type) {
    case Expose:
        if (ev.xexpose.count > 0)
            break;

        WUpdateFrame();
        break;
        
    case ButtonPress:
        w_open = false;
        break;
    }    
}

bool WIsOpen() 
{
    return w_open;
}

void WQuit()
{
    XCloseDisplay(dpy);
}

void WClearFrame()
{
    // TODO: Fix temporay workaround
    // XClearArea(dpy, 0, 0, _win_w, _win_h, false);
    XClearWindow(dpy, win);
}

void WFillRectangle(int x, int y, int w, int h)
{
    XFillRectangle(dpy, win, gc, x, y, w, h);
}
