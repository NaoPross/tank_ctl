#include "serialf.h"
#include "window.h"
#include "data.h"

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

#define DATA_N_MAX 500.0

// debug
int cycle = 0;

void WUpdateFrame()
{
    int i, val, x, y, w, h;
    fprintf(stdout, "INFO: cycle = %d\n", cycle);

    if (cycle > WIN_W) {
        WClearFrame();
        i = cycle % WIN_W * WIN_W;
    }
    else {
        i = 0;
    }

    for (; i < cycle; i++) {
        val = DGetItem(i);

        x = i*BAR_W;
        y = WIN_H - val/DATA_N_MAX * WIN_H;
        w = BAR_W;
        h = val/DATA_N_MAX * WIN_H;

        fprintf(stdout, "INFO: x = %d, y = %d, w = %d, h = %d\n", x, y, w, h);
        fprintf(stdout, "INFO: dData[%d] = %d\n", i, val);
        WFillRectangle(x, y, w, h);
    }
}

/**********************************
 * FUNC MAIN
 **********************************/
int main()
{
    DInit(WIN_W/BAR_W);
    serial_init();

    WCreate(WIN_H, WIN_W);

    char *buf;
    int ibuf;
    buf = (char*) calloc(3, sizeof(char));

    // app loop
    while (WIsOpen()) {
        s_read(buf, 3);
        fprintf(stdout, "INFO: buf = %s\n", buf);

        if (sscanf(buf, "%d", &ibuf) != EOF) {
            fprintf(stdout, "INFO: ibuf = %d\n", ibuf);
            DAddValue(ibuf);
            cycle++;
        }

        WHandleEvents();
    }

    WQuit();
    return 0;
}
