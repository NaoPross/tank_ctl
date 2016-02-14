struct packet
{
	int msg_size;
	char *msg;
};

int send_packet(struct packet);