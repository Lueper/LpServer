#pragma once

#include "LpNetCore.h"
#include "LpBuffer.h"

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

	LpBuffer m_recvBuffer;
	LpBuffer m_sendBuffer;

	uint16_t MAX_SIZE = 8196;
};
}