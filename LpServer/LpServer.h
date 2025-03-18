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

class LpServer {
public:
	LpServer();
	LpServer(const std::string _ip, uint16_t _port);
	~LpServer();

	void SetThreadCount(int _threadCount) { m_threadCount = _threadCount; }
	void SetIOThreadCount(int _ioThreadCount) { m_ioThreadCount = _ioThreadCount; }
	void SetIOBufferSize(int _ioBufferSize) { m_ioBufferSize = _ioBufferSize; }
	void SetSessionPoolSize(int _sessionPoolSize) { m_sessionPoolSize = _sessionPoolSize; }

	void LoadFile(std::string _filePath);
	bool ProcessCommand();

	void Init();
	void Start();
	void Run();
	void Stop();
	void Release();

	void ProcessHandler(int _index);
	void ProcessLog();
	void ProcessServer();

private:
	int m_threadCount = 0;
	int m_ioThreadCount = 0;
	int m_ioBufferSize = 0;
	int m_sessionPoolSize = 0;

	std::atomic<bool> m_running;

	lpnet::LpAcceptor* m_acceptor = nullptr;
	lpnet::LpNetManager* m_netManager = nullptr;
	std::vector<std::thread*> m_asioThreadVector;
	std::vector<std::thread*> m_ioThreadVector;

	std::vector<std::thread> m_handlerThread;
	std::thread m_logThread;
	std::thread m_mainThread;
};