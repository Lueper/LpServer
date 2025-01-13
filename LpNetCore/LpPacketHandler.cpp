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
T* LpPacketHandler::Deserialize(const char* _data, uint32_t _offset) {
	if (_data == nullptr) {
		return nullptr;
	}

	T* data = (T*)malloc(sizeof(T));
	memcpy_s(data, sizeof(T), _data + _offset, sizeof(T));

	return data;
}

void LpPacketHandler::Process(const char* _data, uint32_t _size) {
	if (_size < sizeof(PacketHeader)) {
		return;
	}

	PacketHeader* packetHeader = Deserialize<PacketHeader>(_data);

	if (packetHeader == nullptr) {
		return;
	}

	if (packetHeader->size < _size) {
		return;
	}

	if (packetHeader->size > _size) {
		// '헤더보다 작은 경우' 추가
		// TODO: data를 더 받아와야 하는 경우

		return;
	}

	switch (packetHeader->type) {
		case 100:
			// 100 승인
			break;
		case 101:
			// 김지혁
			break;
		case 102:
			// 유영준
			break;
		case 103:
			// 정은성
			break;
		default:
			return;
	}

	char* packetPayload = reinterpret_cast<char*>(Deserialize<char[128]>(_data + sizeof(PacketHeader)));

	if (packetPayload == nullptr) {
		return;
	}

	PacketTail* packetTail = Deserialize<PacketTail>(_data + sizeof(PacketHeader) + sizeof(char[128]));

	if (packetTail == nullptr) {
		return;
	}

	switch (packetTail->value) {
		case 255:
			// 255 만 승인
			break;
		default:
			return;
	}

	//LpDecryptor.Decode(checkSum);

	std::cout << "[Info]#LpPacketHandler : "
		<< "[type:" << (uint8_t)packetHeader->type << "]"
		<< "[checkSum:" << (uint8_t)packetHeader->checkSum << "]"
		<< "[size:" << (uint32_t)packetHeader->size << "]"
		<< "[payload:" << packetPayload << "]"
		<< "[tail:" << (uint8_t)packetTail->value << "]"
		<< "\n";

	free(packetHeader);
	free(packetPayload);
	free(packetTail);
}
}