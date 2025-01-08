#pragma once

#include <iostream>

#include "LpNetCore.h"

namespace lpnet {
class LpPacketHandler {
public:
	static LpPacketHandler& Instance() {
		static LpPacketHandler instance;
		return instance;
	}

	void Process(const char* _data, uint32_t _size);

private:
	LpPacketHandler();
	~LpPacketHandler();
};
}