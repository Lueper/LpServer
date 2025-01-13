#pragma once

#include <iostream>

#include "LpNetCore.h"
#include "LpPacket.h"

namespace lpnet {
class LpPacketHandler {
public:
	static LpPacketHandler& Instance() {
		static LpPacketHandler instance;
		return instance;
	}

	void Serialize(const char* _data);

	template <typename T>
	T* Deserialize(const char* _data, uint32_t _offset = 0);

	void Process(const char* _data, uint32_t _size);

private:
	LpPacketHandler();
	~LpPacketHandler();
};
}