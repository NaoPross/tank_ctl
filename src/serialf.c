#include "serialf.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/* PACKET
 * [msg_size|*msg]
 *
 * examples:
 * [5|hello]
 * [4|0123]
 */

const char PACKET_START = '[';
const char PACKET_SEPARATOR = '|';
const char PACKET_END = ']';

const char *SERIAL_PORT = "/dev/ttyUSB0";
int fd;

int set_interface_attribs (int fd, int speed, int parity)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (fd, &tty) != 0) {
        fprintf(stderr, "error %d from tcgetattr", errno);
        return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;     // disable break processing
    tty.c_lflag = 0;        // no signaling chars, no echo,
                    // no canonical processing
    tty.c_oflag = 0;        // no remapping, no delays
    tty.c_cc[VMIN]  = 0;        // read doesn't block
    tty.c_cc[VTIME] = 5;        // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0) {
        fprintf(stderr, "error %d from tcsetattr", errno);
        return -1;
    }
    return 0;
}


void set_blocking (int fd, int should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);

    if (tcgetattr (fd, &tty) != 0) {
        fprintf(stderr, "error %d from tggetattr", errno);
        return;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 5;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
        fprintf(stderr, "error %d setting term attributes", errno);
}

void serial_init()
{
    fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        fprintf(stderr, "Error opening %s\n", SERIAL_PORT);
        exit(1);
    }

    set_interface_attribs(fd, B9600, 0);
    set_blocking(fd, 0);
}

void serial_sync(int attempt_max)
{
    int attempt = 0;
    char *tmpchr;

    tmpchr = (char*) calloc(2, sizeof(char));

    while (attempt <= attempt_max) {
        serial_read(tmpchr, 1);
        if (tmpchr[0] == PACKET_END) {
            break;
        }

        attempt++;
    }
}

struct packet read_packet()
{
    struct packet pkt;

    bool done = false;
    size_t text_size, mem_size;

    char *tmpchr;
    char chr;

    char *reading;

    tmpchr = (char*) calloc(1, sizeof(char));

    // reading = (char*) calloc(1, sizeof(char));
    reading = (char*) malloc(sizeof(char));

    while (!done) {
        serial_read(tmpchr, 1);
        chr = tmpchr[0];

        if (chr == '\n' || chr == PACKET_START) {
            // does nothing
        }
        else if (chr == PACKET_SEPARATOR) {
            pkt.msg_size = atoi(reading);
            // reset
            free(reading);
            reading = (char*) malloc(sizeof(char));
        }
        else if (chr == PACKET_END) {
            pkt.msg = reading;
            done = true;
        }
        else {
            // TODO: (fix) this is horrible but it works
            fprintf(stdout, "DEBUG: tmpchar=%s\n", tmpchr);

            text_size = strlen(reading);
            mem_size = sizeof(reading) / sizeof(char);

            if (text_size +1 > mem_size) {
                reading = (char*) realloc(reading, text_size +1);
            }

            reading[text_size++] = chr;
            reading[text_size] = '\0';
        }
    }
    return pkt;
}

void serial_read(char *buf, int size)
{
    char tmpchr;
    int n = 0;
    
    while (n < size) {
        read(fd, &tmpchr, 1);
        
        buf[n] = tmpchr;
        n++;
    }
    buf[n] = '\0';
}
