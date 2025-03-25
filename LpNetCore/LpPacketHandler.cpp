#include "pch.h"
#include "LpPacketHandler.h"

namespace lpnet {
LpPacketHandler::LpPacketHandler() {
	m_sendCount = 0;

	for (int i = 0; i < 100; i++) {
		Packet* packet = (Packet*)malloc(sizeof(Packet));
		m_recvPacketPool.push(packet);
	}
}

LpPacketHandler::~LpPacketHandler() {

}

template <typename T>
char* LpPacketHandler::Serialize(T* _data, uint32_t _offset) {
	if (_data == nullptr) {
		return nullptr;
	}

	return reinterpret_cast<char*>(_data + _offset);
}

template <typename T>
T* LpPacketHandler::Deserialize(char* _data, uint32_t _offset) {
	if (_data == nullptr) {
		return nullptr;
	}

	return reinterpret_cast<T*>(_data + _offset);
}

void LpPacketHandler::Init() {
	for (int i = 0; i < m_ioThreadCount; i++) {
		concurrent_queue<std::pair<int, Packet*>> packetQueue = concurrent_queue<std::pair<int, Packet*>>();
		m_packetQueue.push_back(packetQueue);
	}
}

void LpPacketHandler::Update(int _index) {
	if (m_packetQueue.size() <= _index) {
		return;
	}

	while (m_packetQueue[_index].empty() == false) {
		std::pair<int, Packet*> sessionPacket;
		m_packetQueue[_index].try_pop(sessionPacket);

		Process(sessionPacket.first, sessionPacket.second);

		m_recvPacketPool.push(sessionPacket.second);
	}
}

bool LpPacketHandler::Process(int _sessionID, Packet* _packet) {
	PacketHeader* packetHeader = &_packet->header;
	if (packetHeader == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler packetHeader nullptr");
		return false;
	}

	if (packetHeader->size < sizeof(Packet) || packetHeader->size > sizeof(Packet)) {
		LpLogger::LOG_ERROR("#LpPacketHandler packet size is different");
		return false;
	}

	// type 검사
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

	// seq 검사
	uint32_t seq = GetSequence(_sessionID);
	if (packetHeader->seqNum != 0 && packetHeader->seqNum <= seq) {
		LpLogger::LOG_ERROR("#LpPacketHandler packet seq is not correct");
		return false;
	}
	SetSequence(_sessionID, packetHeader->seqNum);

	char* packetPayload = _packet->payload;
	if (packetPayload == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler packetPayload nullptr");
		return false;
	}

	PacketTail* packetTail = &_packet->tail;
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

	// 2. 패킷 데이터 출력
	//std::ostringstream msg;
	//msg << "#LpPacketHandler Read [ID: " << _sessionID << "]"
	//	<< "[seq: " << (uint32_t)packetHeader->seqNum << "]"
	//	<< "[payload: " << packetPayload << "]";
	//LpLogger::LOG_DEBUG(msg.str());

	return true;
}

void LpPacketHandler::ProcessSend(Packet* _packet, uint32_t _size, char** _data) {
	*_data = Serialize<Packet>(_packet);

	PacketHeader packetHeader = (*_packet).header;

	//std::ostringstream msg;
	//msg << "#LpPacketHandler Send : "
	//	<< "[seq: " << (uint32_t)packetHeader.seqNum << "]"
	//	<< "[type: " << (uint8_t)packetHeader.type << "]"
	//	<< "[checkSum: " << packetHeader.checkSum << "]"
	//	<< "[size: " << (uint32_t)packetHeader.size << "]"
	//	<< "[payload: " << std::string((*_packet).payload) << "]";
	//LpLogger::LOG_DEBUG(msg.str());
}

int LpPacketHandler::PushPacket(int _sessionID, char* _data, uint32_t _size) {
	std::lock_guard<std::mutex> lock(m_mutex);

	if (_data == nullptr) {
		LpLogger::LOG_ERROR("#LpPacketHandler PacketData is null");
	}

	// TODO: 패킷을 Queue에 푸시
	int recvCount = 0;
	uint32_t recvSize = _size;
	uint32_t packetSize = sizeof(Packet);

	while (recvSize >= packetSize) {
		Packet* packet = nullptr;
		if (m_recvPacketPool.try_pop(packet) == false) {
			packet = (Packet*)malloc(sizeof(Packet));
		}

		packet = Deserialize<Packet>(_data, sizeof(Packet) * recvCount);
		m_packetQueue[_sessionID % m_ioThreadCount].push(std::make_pair(_sessionID, packet));

		recvSize -= packetSize;
		recvCount++;
	}

	if (recvSize > 0) {
		// TODO: 남은 Size 처리
	}

	return recvCount;
}

void LpPacketHandler::ResetSequence(int _sessionID) {
	auto iter = m_sessionSequnceMap.find(_sessionID);
	if (iter == m_sessionSequnceMap.end()) {
		m_sessionSequnceMap.insert(std::make_pair(_sessionID, 0));
	}
	else {
		iter->second = 0;
	}
}

uint32_t LpPacketHandler::GetSequence(int _sessionID) {
	auto iter = m_sessionSequnceMap.find(_sessionID);
	if (iter == m_sessionSequnceMap.end()) {
		return -1;
	}
	else {
		return iter->second;
	}
}

void LpPacketHandler::SetSequence(int _sessionID, uint32_t _seq) {
	std::lock_guard<std::mutex> lock(m_mutex);

	auto iter = m_sessionSequnceMap.find(_sessionID);
	if (iter == m_sessionSequnceMap.end()) {
		return;
	}

	iter->second = _seq;
}
}