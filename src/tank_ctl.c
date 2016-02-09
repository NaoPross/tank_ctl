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
#define BAR_W 2

#define DATA_N_MAX 1024.0
#define DATA_N_SIZE 4


int ibuf, cycle = 0;

void WUpdateFrame()
{
    fprintf(stdout, "DEBUG: cycle = %d\n", cycle);

    if (cycle*BAR_W > WIN_W) {
        WClearFrame();
        cycle = 1;
    }

    WFillRectangle(
            cycle*BAR_W,
            WIN_H - ibuf/DATA_N_MAX*WIN_H,
            BAR_W,
            ibuf/DATA_N_MAX*WIN_H
            );
}

/**********************************
 * FUNC MAIN
 **********************************/
int main()
{
    serial_init();
    WCreate(WIN_H, WIN_W);
    
    char *buf;
    buf = (char*) calloc(DATA_N_SIZE, sizeof(char));

    // app loop
    while (WIsOpen()) {
        s_read(buf, DATA_N_SIZE);
        fprintf(stdout, "DEBUG: buf = %s\n", buf);

        if (sscanf(buf, "%d", &ibuf) != EOF) {
            fprintf(stdout, "DEBUG: ibuf = %d\n", ibuf);
            cycle++;
        }

        WHandleEvents();
    }

    WQuit();
    return 0;
}
