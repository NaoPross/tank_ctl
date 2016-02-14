#include "serial.h"

#include <Arduino.h>

const char PACKET_START = '[';
const char PACKET_SEPARATOR = '|';
const char PACKET_END = ']';


int send_packet(struct packet pkt)
{
	// check if the packet does not contain special characters
	for (int i = 0; i < pkt.msg_size; i++) {
		if (pkt.msg[i] == PACKET_START ||
			pkt.msg[i] == PACKET_SEPARATOR ||
			pkt.msg[i] == PACKET_END) {
			return -1;
		}
	}

	String txt = PACKET_START + 
				String(pkt.msg_size) + 
				PACKET_SEPARATOR + 
				pkt.msg + 
				PACKET_END;

	Serial.print(txt);
	return 0;
}