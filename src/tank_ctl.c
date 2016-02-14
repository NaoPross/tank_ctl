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

    // WFillRectangle(
}

/**********************************
 * FUNC MAIN
 **********************************/
int main(int argc, char **argv)
{
    serial_init();
    serial_sync(100);

    WCreate(WIN_H, WIN_W);
    
    // char *buf;
    // buf = (char*) calloc(DATA_N_SIZE, sizeof(char));
    struct packet sensor_data;

    // app loop
    while (WIsOpen()) {
        // serial_read(buf, DATA_N_SIZE);
        // fprintf(stdout, "DEBUG: buf = %s\n", buf);
        sensor_data = read_packet();

        // if (sscanf(buf, "%d", &ibuf) != EOF) {
        if (sscanf(sensor_data.msg, "%d", &ibuf) != EOF) {
            fprintf(stdout, "DEBUG: ibuf = %d\n", ibuf);
            cycle++;
        }

        // this calls WUpdateFrame()
        WHandleEvents();
    }

    WQuit();
    return 0;
}
