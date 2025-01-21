#include "pch.h"
#include "LpPacketHandler.h"

namespace lpnet {
LpPacketHandler::LpPacketHandler() {
	m_sendCount = 0;
}

LpPacketHandler::~LpPacketHandler() {

}

template <typename T>
char* LpPacketHandler::Serialize(const T* _data, uint32_t _offset) {
	if (_data == nullptr) {
		return nullptr;
	}

	char* data = new char[sizeof(T)];
	memcpy_s(data, sizeof(T), _data + _offset, sizeof(T));

	return data;
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
		LpLogger::LOG_ERROR("#LpPacketHandler Packet size is smaller than Header");
		return;
	}

	PacketHeader* packetHeader = Deserialize<PacketHeader>(_data);
	if (packetHeader == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler packetHeader nullptr");
		return;
	}

	//if (packetHeader->size < _size || packetHeader->size > _size) {
	//	// TODO: _data를 다시 받아야 하는 경우
	//	return;
	//}

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
			LpLogger::LOG_ERROR("#LpPacketHandler packet type is not allowed");
			return;
	}

	char* packetPayload = reinterpret_cast<char*>(Deserialize<char[128]>(_data + sizeof(PacketHeader)));
	if (packetPayload == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler packetPayload nullptr");
		return;
	}

	PacketTail* packetTail = Deserialize<PacketTail>(_data + sizeof(PacketHeader) + sizeof(char[128]));
	if (packetTail == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler packetTail nullptr");
		return;
	}

	switch (packetTail->value) {
		case 255:
			// 255 만 승인
			break;
		default:
			LpLogger::LOG_ERROR("#LpPacketHandler packet tail is not allowed");
			return;
	}

	//LpDecryptor.Decode(checkSum);

	m_sendCount++;

	std::ostringstream msg;
	msg << "#LpPacketHandler Read : "
		<< "[type:" << (uint8_t)packetHeader->type << "]"
		<< "[checkSum:" << packetHeader->checkSum << "]"
		<< "[size:" << (uint32_t)packetHeader->size << "]"
		<< "[payload:" << packetPayload << "]"
		<< "[tail:" << (uint8_t)packetTail->value << "]"
		<< " count : " << m_sendCount;

	LpLogger::LOG_INFO(msg.str());

	free(packetHeader);
	free(packetPayload);
	free(packetTail);
}

void LpPacketHandler::ProcessSend(Packet* _packet, uint32_t _size, char** _data) {
	*_data = Serialize<Packet>(_packet);

	m_sendCount++;
	std::ostringstream msg;
	msg << "#LpPacketHandler Send : "
		<< "[type:" << (uint8_t)_packet->header.type << "]"
		<< "[checkSum:" << _packet->header.checkSum << "]"
		<< "[size:" << (uint32_t)_packet->header.size << "]"
		<< "[payload:" << _packet->payload << "]"
		<< "[tail:" << (uint8_t)_packet->tail.value << "]"
		<< " count : " << m_sendCount;

	LpLogger::LOG_INFO(msg.str());
}
}