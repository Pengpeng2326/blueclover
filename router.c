#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <semaphore.h>
#include "router.h"
enum CusIdx {CusIdxPre, CusIdxReg,CusIdxNum};
enum PckIdx {PckIdxVoip, PckIdxdata, PckIdxNum};
Ring ringBuffs[CusIdxNum][PckIdxNum];
sem_t semRing; //use just one semaphore

static int ringPush(Ring* ring, Packet* apacket) {
	if(ring->depth == RING_SIZE) {
		//is overrun
		return -1; 
	}
	ring->buffer[ring->wtIdx] = *apacket;
	ring->wtIdx = (ring->wtIdx+1) % RING_SIZE;
	ring->depth ++;
	return 0;
}

static int ringPop(Ring* ring, Packet** apacket) {
	if(ring->depth>0) {
		*apacket = &ring->buffer[ring->rdIdx];
		ring->rdIdx = (ring->rdIdx+1) % RING_SIZE;
		ring->depth --;
		return 0;
		
	}
	//underrun 
	return -1;
}

void InitRings() {
	memset(ringBuffs,0, sizeof(ringBuffs));

	sem_init(&semRing, 0, 0);
}
void ReceivePacket(uint8_t cusIdx, uint8_t pckIdx, Packet *apacket) {
	if(ringPush(&ringBuffs[cusIdx][pckIdx], apacket)==-1) {
		fprintf(stderr, "buffer[%d][%d] overrun! ", cusIdx,pckIdx);
		//just discard the packet
	}
	else
	{
		sem_post(&semRing);

	}
}

//make the function blocking. 
Packet* ForwardPacket() {
	sem_wait(&semRing);
	uint8_t cusIdx=CusIdxPre, pckIdx=PckIdxVoip;
	Packet *apacket = NULL;
	//always do premium customer first
	while(cusIdx<CusIdxNum) {
		pckIdx = PckIdxVoip;
		//always do Voip packets first
		while(pckIdx<PckIdxNum) {
			//serve the packet. how?
			if(ringPop(&ringBuffs[cusIdx][pckIdx],&apacket)==0) {
				return apacket;
			}
			pckIdx++;
		}
		cusIdx++;
	}

	fprintf(stderr, "undetected underrun! ");
	return NULL;
}
