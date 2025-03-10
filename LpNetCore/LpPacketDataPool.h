#pragma once

#include "LpNetCore.h"

namespace lpnet {
class LpPacketDataPool : public LpPool<char> {
public:
	LpPacketDataPool(int _size);

	char* Alloc();
	char* Pop();
private:
};
}