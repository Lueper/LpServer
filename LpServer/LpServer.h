#pragma once

#pragma comment (lib, "LpNetCore.lib")
#pragma comment (lib, "LpUtility.lib")

#include <iostream>
#include <string>
#include <memory>
#include <thread>

#include "yaml-cpp/yaml.h"

#include "LpNetCore.h"
#include "LpUtility.h"

using namespace boost;

class LpServer {
public:
	LpServer();
	LpServer(const std::string _ip, uint16_t _port);
	~LpServer();

	void SetThreadCount(uint32_t _threadCount) { m_threadCount = _threadCount; }
	void SetIOBufferSize(uint32_t _ioBufferSize) { m_ioBufferSize = _ioBufferSize; }
	void SetSessionPoolSize(uint32_t _sessionPoolSize) { m_sessionPoolSize = _sessionPoolSize; }

	void LoadFile(std::string _filePath);
	bool ProcessCommand();

	void Init();
	void Start();
	void Run();
	void Stop();
	void Release();

	void ProcessIO(int _index);

private:
	uint32_t m_threadCount = 0;
	int m_ioThreadCount = 4;
	uint32_t m_ioBufferSize = 0;
	uint32_t m_sessionPoolSize = 0;

	std::atomic<bool> m_running;

	lpnet::LpAcceptor* m_acceptor = nullptr;
	std::vector<std::thread*> m_asioThreadVector;
	std::vector<std::thread*> m_ioThreadVector;
};