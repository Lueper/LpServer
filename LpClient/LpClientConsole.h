#pragma once

#pragma comment (lib, "LpNetCore.lib")
#pragma comment (lib, "LpUtility.lib")

#include <ppl.h>
#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <concurrent_vector.h>

#include "LpNetCore.h"
#include "LpUtility.h"

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/system.hpp"

#include "yaml-cpp/yaml.h"
#include "LpPacket.h"

#include "LpClient.h"

class LpClient;

namespace lpnet {
class LpClientConsole {
public:
	LpClientConsole();
	~LpClientConsole();

	void InitCommand();
	bool ProcessCommand();

	void LoadFile(std::string _filePath);
	void SetIOBufferSize(uint32_t _ioBufferSize) { m_ioBufferSize = _ioBufferSize; }
	void SetThreadCount(int _threadCount) { m_threadCount = _threadCount; }
	void SetSessionCount(int _sessionCount) { m_sessionCount = _sessionCount; }
	void SetSessionPoolSize(int _sessionPoolSize) { m_sessionPoolSize = _sessionPoolSize; };
	void SetClientReconnectCount(int _count) { m_clientReconnectCount = _count; };
	void SetSessionSendCount(int _count) { m_sessionSendCount = _count; };
	void ClientMain();

	void SetServerCount(uint32_t _serverCount) { m_serverCount = _serverCount; }
	void SetSendIndex(uint32_t _sendIndex) { m_sendIndex = _sendIndex; }


	void ProcessClient(int index);
	void ProcessLog();

	void Run();
	void Stop();

private:
	uint32_t m_ioBufferSize;
	int m_threadCount;
	int m_sessionCount;
	int m_sessionPoolSize;

	int m_clientReconnectCount;
	int m_sessionSendCount;
	
	std::vector<std::thread*> m_asioThreadVector;
	concurrency::concurrent_vector<std::pair<std::thread*, LpClient*>> m_clientThreadVector;

	int m_serverCount;
	int m_sendIndex;
	std::vector<std::pair<std::string, uint16_t>> m_serverList;
	std::pair<std::string, uint16_t> m_connectServer;

	std::mutex m_mutex;





	std::atomic<bool> m_running;
	std::vector<LpClient*> m_clientVector;
	std::vector<std::thread> m_clientWorkVector;
};
}