#pragma once

#include <iostream>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>

#include "LpSingleton.h"
#include "LpNetCore.h"
#include "LpPacket.h"

namespace lpnet {
	using namespace Concurrency;
class LpPacketHandler : public LpSingleton<LpPacketHandler> {
public:
	LpPacketHandler();
	~LpPacketHandler();

	template <typename T>
	char* Serialize(T* _data, uint32_t _offset = 0);
	template <typename T>
	T* Deserialize(char* _data, uint32_t _offset = 0);

	void Init();
	void Update(int _index);

	bool Process(int _sessionID, Packet* _packet);
	void ProcessSend(Packet* _packet, uint32_t _size, char** _data);

	int PushPacket(int _sessionID, char* _data, uint32_t _size);

	void ResetSequence(int _sessionID);
	uint32_t GetSequence(int _sessionID);
	void SetSequence(int _sessionID, uint32_t _seq);

	void SetIOThreadCount(int _index) { m_ioThreadCount = _index; };

	concurrent_queue<Packet*> m_recvPacketPool;
	std::vector<concurrent_queue<std::pair<int, Packet*>>> m_packetQueue;

	concurrent_unordered_map<int, uint32_t> m_sessionSequnceMap;

	std::atomic<int> m_sendCount;
	
	int m_ioThreadCount;

	std::mutex m_mutex;
};
}