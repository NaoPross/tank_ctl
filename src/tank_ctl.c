#include "serialf.h"

#include <X11/Xlib.h>

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef TERMIOS_H
#define TERMIOS_H
#include <termios.h>
#endif

#ifndef FCNTL_H
#define FCNTL_H
#include <fcntl.h>
#endif

#ifndef UNISTD_H
#define UNISTD_H
#include <unistd.h>
#endif

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif



#define WIN_H 400
#define WIN_W 600
#define BAR_W 3

/**********************************
 * SETUP
 **********************************/

// serial
const char *serial_port = "/dev/ttyUSB0";
int fd;

// data for the graph
int graph_data[WIN_W/BAR_W];
int graph_arr_size, graph_data_p;

void var_init()
{
    memset(graph_data, 0, sizeof(graph_data));

    graph_arr_size = WIN_W/BAR_W;
    graph_data_p = 0;
}

void serial_init()
{
    fd = open(serial_port, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        fprintf(stderr, "Error opening %s\n", serial_port);
        exit(1);
    }

    set_interface_attribs(fd, B9600, 0);
    set_blocking(fd, 0);
}

/**********************************
 * OTHER FUNCTIONS
 **********************************/

// Serial sync
// TODO: This shit is not working
void serial_sync()
{
    char c;

    for (;;) {
        read(fd, c, sizeof(char));

        fprintf(stdout, "sync: got [%c] from serial\n", c);

        if (c == '\n')
            break;

        usleep(10000);
    }
}

// TODO: finish this (untested and probably not working)
int get_serial_data()
{
    char c;
    int num = 0;

    for (int i = 3; i < 0; i--) {
        read(fd, c, sizeof(char));
        
        if (c == '\n') {
            fprintf(stderr, "ERROR: serial out of sync\n");
        }
        else {
            int n = c - '0';
            num += n * pow(10, i);
        }
    }

    return num;
}

// Add `value` to the `graph_data` array stack
void g_arr_add(int value)
{
    if (graph_data_p > graph_arr_size) {
        graph_data_p = 0;
    } else {
        graph_data_p++;
    }

    graph_data[graph_data_p] = value;
}

// Xlib window draw
void win_draw(Display *dpy, Window win, GC gc)
{
    for (int i = 0; i < graph_arr_size; i++) {
        XFillRectangle(dpy, win, gc,
                i*BAR_W,
                WIN_H - graph_data[i], 
                BAR_W,
                graph_data[i]);
    }
}

/**********************************
 * FUNC MAIN
 **********************************/
int main()
{
    var_init();
    serial_init();

    Display *dpy;
    Window rootwin;
    Window win;

    Colormap cmap;
    XEvent e;
    GC gc;

    int scr;

    // Try to connect to XServer
    if (!(dpy=XOpenDisplay(NULL))) {
        fprintf(stderr, "ERROR: Could not open display\n");
        exit(1);
    }

    // Create Window
    scr = DefaultScreen(dpy);
    rootwin = RootWindow(dpy, scr);
    cmap = DefaultColormap(dpy, scr);
    
    win = XCreateSimpleWindow(dpy, rootwin, 1, 1, WIN_W, WIN_H, 0,
            BlackPixel(dpy, scr), BlackPixel(dpy, scr));

    XStoreName(dpy, win, "floating");
    
    // Create graphic context
    gc = XCreateGC(dpy, win, 0, NULL);
    XSetForeground(dpy, gc, WhitePixel(dpy, scr));
    XSelectInput(dpy, win, ExposureMask|ButtonPressMask);

    // Connect the window to the XServer
    XMapWindow(dpy, win);

    // App Loop
    for (;;) {
        // try to sync with the serial
        serial_sync();

        
        // Handle XEvents
        XCheckMaskEvent(dpy, ExposureMask|ButtonPressMask, &e);
        if (e.type == Expose && e.xexpose.count < 1) {
            // Draw
            win_draw(dpy, win, gc);
        }
        // Close on mouseclick
        else if (e.type == ButtonPress) {
            break;
        }
    }

    XCloseDisplay(dpy);

    return 0;
}
