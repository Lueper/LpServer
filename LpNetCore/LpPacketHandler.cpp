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

	if (packetHeader->size < _size || packetHeader->size > _size) {
		//LpLogger::LOG_ERROR("#LpPacketHandler packet size is different");
		return;
	}

	switch (packetHeader->type) {
		case 100:
			// 100 Ω¬¿Œ
			break;
		case 101:
			// ±Ë¡ˆ«ı
			break;
		case 102:
			// ¿Øøµ¡ÿ
			break;
		case 103:
			// ¡§¿∫º∫
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
			// 255 ∏∏ Ω¬¿Œ
			break;
		default:
			LpLogger::LOG_ERROR("#LpPacketHandler packet tail is not allowed");
			return;
	}

	//LpDecryptor.Decode(checkSum);

	m_sendCount.fetch_add(1);

	std::ostringstream msg;
	msg << "#LpPacketHandler Read : "
		<< "[type:" << (uint8_t)packetHeader->type << "]"
		<< "[checkSum:" << packetHeader->checkSum << "]"
		<< "[size:" << (uint32_t)packetHeader->size << "]"
		<< "[payload:" << packetPayload << "]"
		<< "[tail:" << (uint8_t)packetTail->value << "]"
		<< " count : " << m_sendCount;

	LpLogger::LOG_DEBUG(msg.str());

	free(packetHeader);
	free(packetPayload);
	free(packetTail);

	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void LpPacketHandler::ProcessSend(Packet* _packet, uint32_t _size, char** _data) {
	*_data = Serialize<Packet>(_packet);
}
}