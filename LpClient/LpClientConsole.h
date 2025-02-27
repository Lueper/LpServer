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
	void SetThreadCount(uint32_t _threadCount) { m_threadCount = _threadCount; }
	void SetSessionCount(uint32_t _sessionCount) { m_sessionCount = _sessionCount; }
	void SetSessionPoolSize(uint32_t _sessionPoolSize) { m_sessionPoolSize = _sessionPoolSize; };
	void ClientMain();

	void SetServerCount(uint32_t _serverCount) { m_serverCount = _serverCount; }
	void SetSendIndex(uint32_t _sendIndex) { m_sendIndex = _sendIndex; }

	void ProcessClient(int index);
	void ProcessLog();

	void Run();
	void Stop();

private:
	uint32_t m_ioBufferSize;
	uint32_t m_threadCount;
	uint32_t m_sessionCount;
	uint32_t m_sessionPoolSize;
	
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