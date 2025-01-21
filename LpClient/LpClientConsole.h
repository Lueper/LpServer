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

#include "LpPacket.h"

#include "LpClient.h"

class LpClient;

namespace lpnet {
class LpClientConsole {
public:
	LpClientConsole();
	~LpClientConsole();

	void ClientMain();

	void InitCommand();
	bool ProcessCommand();

private:
	uint32_t m_threadCount;
	uint32_t m_sendCount;
	uint32_t m_sessionCount;
	std::vector<std::thread*> m_threadVector;
	std::vector<LpClient*> m_ClientVector;

	std::mutex m_mutex;
};
}