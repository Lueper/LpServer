#pragma once

#include <iostream>

#include "LpSingleton.h"
#include "LpNetCore.h"
#include "LpPacket.h"

namespace lpnet {
class LpPacketHandler : public LpSingleton<LpPacketHandler> {
public:
	LpPacketHandler();
	~LpPacketHandler();

	template <typename T>
	char* Serialize(const T* _data, uint32_t _offset = 0);
	template <typename T>
	T* Deserialize(const char* _data, uint32_t _offset = 0);

	void Process(const char* _data, uint32_t _size);
	void ProcessSend(Packet* _packet, uint32_t _size, char** _data);

	int m_sendCount;
};
}