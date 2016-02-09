#include "window.h"

bool w_open;
int scr, fd;
int _win_h, _win_w;

Display *dpy;
Window rootwin;
Window win;
XEvent ev;

cairo_surface_t *cs;

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

    // Set window trigger events
    XSelectInput(dpy, win, ExposureMask | ButtonPressMask);

    // Create a cairo surface
    cs = cairo_xlib_surface_create(dpy, win, DefaultVisual(dpy, 0), win_w, win_h);

    // Connect the window to the XServer
    XStoreName(dpy, win, "tank_ctl - serial");
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
    cairo_t *c;
    c = cairo_create(cs);

    cairo_rectangle(c, 0, 0, _win_w, _win_h);
    cairo_set_source_rgb(c, 0, 0, 0);
    cairo_fill(c);
    cairo_show_page(c);
    cairo_destroy(c);
}

void WFillRectangle(int x, int y, int w, int h)
{
    cairo_t *c;
    c = cairo_create(cs);
    
    cairo_rectangle(c, x, y, w, h);
    cairo_set_source_rgb(c, 1, 1, 1);
    cairo_fill(c);
    
    cairo_show_page(c);
    cairo_destroy(c);
}
