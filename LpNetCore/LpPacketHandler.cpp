#include "pch.h"
#include "LpPacketHandler.h"

namespace lpnet {
LpPacketHandler::LpPacketHandler() {

}

LpPacketHandler::~LpPacketHandler() {

}

void LpPacketHandler::Serialize(const char* _data) {

}

template <typename T>
T* LpPacketHandler::Deserialize(const char* _data) {
	if (_data == nullptr) {
		return nullptr;
	}

	uint16_t offset = 0;
	T* data = nullptr;
	memset(data, 0, sizeof(T));

	memcpy_s(data, sizeof(data), _data + offset, sizeof(data));

	return data;
}

void LpPacketHandler::Process(const char* _data, uint32_t _size) {
	if (_size >= sizeof(PacketHeader) + sizeof(PacketTail))

	if (packetHeader->size < _size) {
		return;
	}

	PacketHeader* packetHeader = Deserialize<PacketHeader>(_data);

	if (packetHeader == nullptr) {
		return;
	}

	if (packetHeader->size > _size) {
		// '헤더보다 작은 경우' 추가
		// TODO: data를 더 받아와야 하는 경우

		return;
	}

	switch (packetHeader->type) {
		default:
		break;
	}

	

	//std::cout << packet->header.type << "\n";
}
}