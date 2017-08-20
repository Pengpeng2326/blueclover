#include <stdio.h>
#include "router.h"
int main(){
	printf("this is a test function \r\n");

	int packetNum = 100;
	InitRings();

	for(int packIdx=0;packIdx<packetNum;packIdx++) {
		Packet apacket;

		apacket.byte = packIdx&0xFF;	
		ReceivePacket(packIdx%2, packIdx%4>>1, &apacket);
		printf("received a packet %d %d %d\r\n", packIdx%2, packIdx%4>>1, apacket.byte);
		Packet *receivedPacket;
		receivedPacket = ForwardPacket();
		if(receivedPacket == NULL) printf("error, received a NULL pointer \r\n");
		printf("forwarded a packet %d\r\n", receivedPacket->byte);

	}

}

