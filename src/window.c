#include "window.h"

bool w_open;
int scr;

Display *dpy;
Window rootwin;
Window win;
Colormap cmap;
GC gc;
XEvent ev;

void WCreate(int win_h, int win_w)
{
    // Try to connect to XServer
    if (!(dpy=XOpenDisplay(NULL))) {
        fprintf(stderr, "ERROR: Could not open display\n");
        exit(1);
    }

    // Create Window
    scr = DefaultScreen(dpy);
    rootwin = RootWindow(dpy, scr);
    cmap = DefaultColormap(dpy, scr);
    
    win = XCreateSimpleWindow(dpy, rootwin, 1, 1, win_w, win_h, 0,
            BlackPixel(dpy, scr), BlackPixel(dpy, scr));

    XStoreName(dpy, win, "floating");
    
    // Create graphic context
    gc = XCreateGC(dpy, win, 0, NULL);
    XSetForeground(dpy, gc, WhitePixel(dpy, scr));
    XSelectInput(dpy, win, ExposureMask|ButtonPressMask);

    // Connect the window to the XServer
    XMapWindow(dpy, win);

    w_open = true;
}

void WHandleEvents() 
{
    XCheckMaskEvent(dpy, ExposureMask|ButtonPressMask, &ev);
    if (ev.type == Expose && ev.xexpose.count < 1) {
        // Draw
        WUpdateFrame();
    }
    // Close on mouseclick
    else if (ev.type == ButtonPress) {
        w_open = false;
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

}

void WFillRectangle(int x, int y, int w, int h)
{
    XFillRectangle(dpy, win, gc, x, y, w, h);
}
