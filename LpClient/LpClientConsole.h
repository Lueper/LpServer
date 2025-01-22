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
	void SetThreadCount(uint32_t _threadCount) { m_threadCount = _threadCount; }
	void SetIOBufferSize(uint32_t _ioBufferSize) { m_ioBufferSize = _ioBufferSize; }
	void ClientMain();

private:
	uint32_t m_threadCount;
	uint32_t m_sendCount;
	uint32_t m_ioBufferSize;
	uint32_t m_sessionCount;
	
	std::vector<std::thread*> m_threadVector;
	std::vector<LpClient*> m_ClientVector;

	std::pair<std::string, uint16_t> YeongjunServer;
	std::pair<std::string, uint16_t> EunseongServer;

	std::mutex m_mutex;
};
}