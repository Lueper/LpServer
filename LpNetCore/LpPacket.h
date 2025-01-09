#pragma once

#include <iostream>

// 21 bytes
struct PacketHeader {
	uint8_t type;
	char checkSum[16];
	uint32_t size;
};

// 1 byte
struct PacketTail {
	uint8_t value;
};

// 150 bytes
struct Packet {
	PacketHeader header;
	char		 payload[128];
	PacketTail	 tail;
};