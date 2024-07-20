#pragma once

#pragma comment (lib, "LpNetCore.lib")

#include <iostream>
#include <string>
#include <memory>

#include "LpNetCore.h"

using namespace boost;

class LpServer {
public:
	LpServer();
	LpServer(const std::string _ip, uint16_t _port);
	~LpServer();

	void Run();

private:
	//std::vector<lpnet::LpAcceptor> m_vecAcceptor;
	lpnet::LpAcceptor* m_acceptor = nullptr;
	//lpnet::LpSession m_session;
};