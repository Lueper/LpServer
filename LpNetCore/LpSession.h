#pragma once

#include "LpNetCore.h"
#include "LpBuffer.h"

namespace lpnet {
	using namespace boost;
class LpSession {
public:
	LpSession();
	LpSession(uint32_t _size);
	~LpSession();

	void Close();
	void Read();
	void OnRead(const system::error_code& _error, uint32_t _size);
	void Write();
	void OnWrite(const system::error_code& _error, uint32_t _size);

	asio::ip::tcp::socket& GetSocket();
private:
	asio::ip::tcp::socket m_socket;

	LpBuffer m_recvBuffer;
	LpBuffer m_sendBuffer;
};
}