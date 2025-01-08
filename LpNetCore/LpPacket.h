#pragma once

#include <iostream>

// 9 bytes
struct PacketHeader {
	uint8_t type;
	char checkSum[4];
	uint32_t size;
};

// 138 bytes
struct Packet {
	PacketHeader header;
	char		 payload[128];
	PacketTail	 tail;
};

// 1 byte
struct PacketTail {
	uint8_t value;
};