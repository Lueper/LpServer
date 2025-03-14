#include "pch.h"
#include "LpPacketHandler.h"

namespace lpnet {
LpPacketHandler::LpPacketHandler() {
	m_sendCount = 0;
}

LpPacketHandler::~LpPacketHandler() {

}

template <typename T>
//char* LpPacketHandler::Serialize(const T* _data, uint32_t _offset) {
char* LpPacketHandler::Serialize(T* _data, uint32_t _offset) {
	if (_data == nullptr) {
		return nullptr;
	}

	//char* data = new char[sizeof(T)];
	//memcpy_s(data, sizeof(T), _data + _offset, sizeof(T));

	//return data;

	return reinterpret_cast<char*>(_data);
}

template <typename T>
//T* LpPacketHandler::Deserialize(const char* _data, uint32_t _offset) {
T* LpPacketHandler::Deserialize(char* _data, uint32_t _offset) {
	if (_data == nullptr) {
		return nullptr;
	}

	//T* data = (T*)malloc(sizeof(T));
	//memcpy_s(data, sizeof(T), _data + _offset, sizeof(T));

	//return data;

	return reinterpret_cast<T*>(_data);
}

//void LpPacketHandler::Process(const char* _data, uint32_t _size) {
bool LpPacketHandler::Process(char* _data, uint32_t _size) {
	if (_size < sizeof(PacketHeader)) {
		LpLogger::LOG_ERROR("#LpPacketHandler Packet size is smaller than Header");
		return false;
	}

	PacketHeader* packetHeader = Deserialize<PacketHeader>(_data);
	if (packetHeader == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler packetHeader nullptr");
		return false;
	}

	if (packetHeader->size < _size || packetHeader->size > _size) {
		LpLogger::LOG_ERROR("#LpPacketHandler packet size is different");
		return false;
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
			LpLogger::LOG_ERROR("#LpPacketHandler packet type is not allowed");
			return false;
	}

	char* packetPayload = reinterpret_cast<char*>(Deserialize<char[128]>(_data + sizeof(PacketHeader)));
	if (packetPayload == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler packetPayload nullptr");
		return false;
	}

	PacketTail* packetTail = Deserialize<PacketTail>(_data + sizeof(PacketHeader) + sizeof(char[128]));
	if (packetTail == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler packetTail nullptr");
		return false;
	}

	switch (packetTail->value) {
		case 255:
			// 255 만 승인
			break;
		default:
			LpLogger::LOG_ERROR("#LpPacketHandler packet tail is not allowed");
			return false;
	}

	//LpDecryptor.Decode(checkSum);

	// 2. 패킷 데이터 출력
	//std::ostringstream msg;
	//msg << "#LpPacketHandler Read : "
	//	<< "[seq: " << (uint32_t)packetHeader->seqNum << "]"
	//	<< "[payload: " << packetPayload << "]";
	//LpLogger::LOG_DEBUG(msg.str());

	//free(packetHeader);
	//free(packetPayload);
	//free(packetTail);

	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return true;
}

bool LpPacketHandler::Process(int _sessionID, char* _data, uint32_t _size) {
	if (_size < sizeof(PacketHeader)) {
		LpLogger::LOG_ERROR("#LpPacketHandler Packet size is smaller than Header");
		return false;
	}

	PacketHeader* packetHeader = Deserialize<PacketHeader>(_data);
	if (packetHeader == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler packetHeader nullptr");
		return false;
	}

	if (packetHeader->size < _size || packetHeader->size > _size) {
		//LpLogger::LOG_ERROR("#LpPacketHandler packet size is different");
		return false;
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
		LpLogger::LOG_ERROR("#LpPacketHandler packet type is not allowed");
		return false;
	}

	char* packetPayload = reinterpret_cast<char*>(Deserialize<char[128]>(_data + sizeof(PacketHeader)));
	if (packetPayload == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler packetPayload nullptr");
		return false;
	}

	PacketTail* packetTail = Deserialize<PacketTail>(_data + sizeof(PacketHeader) + sizeof(char[128]));
	if (packetTail == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler packetTail nullptr");
		return false;
	}

	switch (packetTail->value) {
	case 255:
		// 255 만 승인
		break;
	default:
		LpLogger::LOG_ERROR("#LpPacketHandler packet tail is not allowed");
		return false;
	}

	//LpDecryptor.Decode(checkSum);

	// 2. 패킷 데이터 출력
	//std::ostringstream msg;
	//msg << "#LpPacketHandler Read [ID: " << _sessionID << "]"
	//	<< "[seq: " << (uint32_t)packetHeader->seqNum << "]"
	//	<< "[payload: " << packetPayload << "]";
	//LpLogger::LOG_DEBUG(msg.str());

	//free(packetHeader);
	//free(packetPayload);
	//free(packetTail);

	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return true;
}

void LpPacketHandler::ProcessSend(Packet* _packet, uint32_t _size, char** _data) {
	*_data = Serialize<Packet>(_packet);

	PacketHeader packetHeader = (*_packet).header;

	std::ostringstream msg;
	msg << "#LpPacketHandler Send : "
		<< "[seq: " << (uint32_t)packetHeader.seqNum << "]"
		<< "[type: " << (uint8_t)packetHeader.type << "]"
		<< "[checkSum: " << packetHeader.checkSum << "]"
		<< "[size: " << (uint32_t)packetHeader.size << "]"
		<< "[payload: " << std::string((*_packet).payload) << "]";
	LpLogger::LOG_DEBUG(msg.str());
}

int LpPacketHandler::PushPacket(int _sessionID, char* _data, uint32_t _size) {
	if (_data == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler PacketData is null");
	}

	// TODO: 패킷을 Queue에 푸시
}
}