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
	T* data = (T*)malloc(sizeof(T));
	memset(data, 0, sizeof(T));

	memcpy_s(data, sizeof(T), _data + offset, sizeof(T));

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
		// '������� ���� ���' �߰�
		// TODO: data�� �� �޾ƿ;� �ϴ� ���

		return;
	}

	switch (packetHeader->type) {
		case 100:
			// �ϴ� 100 �� ����
			break;
		default:
			return;
	}

	if (_size != sizeof(Packet)) {
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
			// 255 �� ����
			break;
		default:
			return;
	}

	//LpDecryptor.Decode(checkSum)

	std::cout << "[Info]#LpPacketHandler : "
		<< "[type:" << (uint8_t)packetHeader->type << "]"
		<< "[size:" << (uint32_t)packetHeader->size << "]"
		<< "[payload:" << packetPayload << "]"
		<< "\n";

	delete packetHeader;
	delete packetPayload;
	delete packetTail;
}
}