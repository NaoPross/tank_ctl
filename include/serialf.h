#pragma once

struct packet
{
    int msg_size;
    char *msg;
};

int set_interface_attribs(int, int, int);
void set_blocking(int, int);

void serial_init();
void serial_sync(int);
struct packet read_packet();
void serial_read();
