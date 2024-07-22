#pragma once

#include "LpNetCore.h"

namespace lpnet {
	using namespace boost;
class LpSession {
public:
	LpSession();
	~LpSession();

	void Close();
	void Read();
	void OnRead(const system::error_code& _error);
	void Write();
	void OnWrite(const system::error_code& _error);

	asio::ip::tcp::socket& GetSocket();
private:
	asio::ip::tcp::socket m_socket;

	char* m_recvBuffer = nullptr;
	char* m_sendBuffer = nullptr;
};
}