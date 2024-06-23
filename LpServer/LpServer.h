#pragma once

#include <iostream>
#include <memory>

#include "LpNetCore.h"

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/system.hpp"

using namespace boost;

class LpServer {
public:
	static const size_t g_bufferSize = 16384;

	LpServer();
	~LpServer();

	void Run();

	void Accept();
	void OnAccept(const system::error_code& _error);
	void Read();
	void OnRead(size_t _size, const system::error_code& _error);

	asio::io_service* GetIoService() { return m_ioService; };

private:
	asio::io_service* m_ioService = nullptr;
	asio::ip::tcp::endpoint* m_endpoint = nullptr;
	asio::ip::tcp::socket* m_socket = nullptr;
	asio::ip::tcp::acceptor* m_acceptor = nullptr;
	asio::io_service::work* m_ioWork = nullptr;
	asio::ip::tcp::resolver* m_resolver = nullptr;

	char m_recvBuffer[g_bufferSize];
};