#pragma once

#pragma comment (lib, "LpNetCore.lib")
#pragma comment (lib, "LpUtility.lib")

#include <ppl.h>
#include <iostream>
#include <string>
#include <memory>
#include <thread>
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
	void Init(uint32_t _threadCount, uint32_t _sessionCount);
	void Run();
	void Connect(const std::string _ip, uint16_t _port);
	void OnConnect(lpnet::LpSession* _session, const system::error_code& _error);
	void Send(Packet* _packet, uint32_t _size);
	void Close();
	void Stop();
	void Release();

	void SetThreadCount(uint32_t _threadCount) { m_threadCount = _threadCount; };
	void SetSessionCount(uint32_t _sessionCount) { m_sessionCount = _sessionCount; };

	void TestSend();
private:
	asio::ip::tcp::resolver* m_resolver;
	asio::ip::tcp::endpoint* m_endPoint;

	lpnet::LpAcceptor* m_acceptor = nullptr;
	lpnet::LpSession* m_session = nullptr;

	std::vector<std::thread*> m_threadVector;

	uint32_t m_threadCount;
	uint32_t m_sessionCount;
};
}