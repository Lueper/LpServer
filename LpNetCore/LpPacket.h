#pragma once

#include <iostream>

#pragma pack(push, 1)
// 25 bytes
struct PacketHeader {
	uint32_t seqNum;
	uint8_t type;
	char checkSum[16];
	uint32_t size;
};

// 1 byte
struct PacketTail {
	uint8_t value;
};

// 154 bytes
struct Packet {
	PacketHeader header;
	char		 payload[128];
	PacketTail	 tail;
};
#pragma pack(pop)