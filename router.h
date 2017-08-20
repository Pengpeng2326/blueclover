#include <stdint.h>
typedef struct Packet_t {
	uint8_t byte;
} Packet;

#define RING_SIZE (256)
typedef struct Ring_t {
	int wtIdx;
	int rdIdx;
	uint32_t depth;
	//bool underrun;//no need
	Packet buffer[RING_SIZE];
} Ring;



void InitRings();
void ReceivePacket(uint8_t cusIdx, uint8_t pckIdx, Packet *apacket); 

Packet* ForwardPacket(); 
