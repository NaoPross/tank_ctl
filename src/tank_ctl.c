#include "serialf.h"
#include "window.h"

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#define WIN_H 480
#define WIN_W 620
#define BAR_W 5


// data for the graph
int graph_data[WIN_W/BAR_W];
int graph_arr_size, graph_data_p;

void var_init()
{
    memset(graph_data, 0, sizeof(graph_data));

    graph_arr_size = WIN_W/BAR_W;
    graph_data_p = 0;
}

void g_arr_add(int value)
{
    if (graph_data_p > graph_arr_size) {
        graph_data_p = 0;
    } else {
        graph_data_p++;
    }

    graph_data[graph_data_p] = value;
}


void WUpdateFrame()
{
    for (int i = 0; i < graph_arr_size; i++) {
        WFillRectangle(
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

    WCreate(WIN_H, WIN_W);

    // testing {{{
    char buf[16];
    int ibuf;
    // }}}

    // app loop
    while (WIsOpen()) {
        WHandleEvents();
        WUpdateFrame();
        
        // testing {{{
        s_read(buf, 8);
        int conv = sscanf(buf, "%d", &ibuf);

        if (!conv || conv == EOF) {
            // fprintf(stderr, "ERROR: failed to convert input\n");
        }
        else if (ibuf != 0) {
            // fprintf(stdout, "CHR: %s\n", buf);
            fprintf(stdout, "%d\n", ibuf);
        }
        // }}}
    }

    WQuit();
    return 0;
}
