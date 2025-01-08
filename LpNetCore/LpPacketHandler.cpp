#include "pch.h"
#include "LpPacketHandler.h"

namespace lpnet {
LpPacketHandler::LpPacketHandler() {

}

LpPacketHandler::~LpPacketHandler() {

}

void LpPacketHandler::Process(const char* _data, uint32_t _size) {
	std::cout << "[Info]#PacketHandler : Process - [size: " << _size << "][data: " << std::string(_data) << "]" << "\n";
}
}