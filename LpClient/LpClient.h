#pragma once

#pragma comment (lib, "LpNetCore.lib")
#pragma comment (lib, "LpUtility.lib")

#include <ppl.h>
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <random>
#include <concurrent_unordered_map.h>

#include "LpNetCore.h"
#include "LpUtility.h"

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/system.hpp"

#include "LpPacket.h"

#include "LpClientConsole.h"

namespace lpnet {
	using namespace boost;
class LpClient {
public:
	LpClient();
	~LpClient();

	//void Init();
	void Init(uint32_t _threadCount, uint32_t _sessionCount, uint32_t _ioBufferSize, uint32_t _sessionPoolSize, int _sessionSendCount);
	void Start();
	void Run();
	void Connect(const std::string _ip, uint16_t _port);
	void OnConnect(lpnet::LpSession* _session, const system::error_code& _error);
	void Send(Packet* _packet, uint32_t _size);
	void Close();
	void Stop();
	void Release();
	void AsyncWait();
	void OnWait(const system::error_code& _error);
	void CloseSessions();
	void CheckSessions();
	
	void SetIOBufferSize(uint32_t _ioBufferSize) { m_ioBufferSize = _ioBufferSize; };
	void SetThreadCount(uint32_t _threadCount) { m_threadCount = _threadCount; };
	void SetSessionCount(uint32_t _sessionCount) { m_sessionCount = _sessionCount; };
	void SetSessionPoolSize(uint32_t _size) { m_sessionPoolSize = _size; };
	
	void SetSessionSendCount(int _count) { m_sessionSendCount = _count; };

	void TestSend();

	uint32_t m_sendCount = 0;
private:
	asio::ip::tcp::resolver* m_resolver;
	asio::ip::tcp::endpoint* m_endPoint;
	asio::steady_timer* m_timer;

	asio::io_context::work* m_work;

	lpnet::LpAcceptor* m_acceptor = nullptr;
	lpnet::LpSession* m_session = nullptr;
	lpnet::LpSessionPool* m_sessionPool= nullptr;

	std::vector<std::thread*> m_asioThreadVector;
	std::vector<LpSession*> m_sessionVector;
	std::unordered_map<int, LpSession*> m_sessionMap;

	uint32_t m_ioBufferSize;
	uint32_t m_threadCount;
	uint32_t m_sessionCount;
	uint32_t m_sessionPoolSize;

	uint32_t m_connectTryCount = 0;
	uint32_t m_sequenceNumber = 0;
	int m_sessionSendCount = 0;
};
}