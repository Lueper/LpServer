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

	void Init();
	void Connect(const std::string _ip, uint16_t _port);
	void Send(void* _buffer, uint32_t _size, uint32_t& sendSize);

	asio::ip::tcp::socket& GetSocket();
	void SetIOBufferSize(uint32_t _size) { m_ioBufferSize = _size; };
private:
	asio::ip::tcp::socket m_socket;
	asio::ip::tcp::endpoint m_endpoint;

	char* m_recvBuffer;
	char* m_sendBuffer;
	LpBuffer m_readBuffer;
	LpBuffer m_writeBuffer;

	uint32_t m_ioBufferSize;
};
}