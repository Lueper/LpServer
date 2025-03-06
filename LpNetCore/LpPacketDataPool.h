#pragma once

#include "LpNetCore.h"

namespace lpnet {
class LpPacketDataPool : public LpPool<char*> {
public:
	void Alloc();
private:

};
}