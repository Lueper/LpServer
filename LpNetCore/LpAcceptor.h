#pragma once

#include "LpNetCore.h"
#include "LpIOContext.h"

namespace lpnet {
	using namespace boost;
class LpAcceptor {
public:
	LpAcceptor();
	~LpAcceptor();

	void Bind(const std::string ip, uint16_t port);
	void Listen(int32_t backLog = asio::ip::tcp::acceptor::max_listen_connections);
	void Accept(asio::ip::tcp::socket& socket);
	void OnAccept(const system::error_code& _error);
	void Close();

private:
	asio::ip::tcp::acceptor m_acceptor;
};
}