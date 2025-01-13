#include "pch.h"
#include "LpAcceptor.h"

namespace lpnet {
LpAcceptor::LpAcceptor() {
	m_acceptor = new asio::ip::tcp::acceptor(*LpIOContext::GetIOContext());
}

LpAcceptor::~LpAcceptor() {
	delete m_acceptor;
}

void LpAcceptor::Bind(const std::string _ip, uint16_t _port) {
	system::error_code error;

	asio::ip::address address = asio::ip::make_address(_ip);
	asio::ip::tcp::endpoint endpoint(address, _port);
	m_acceptor->open(endpoint.protocol(), error);
	m_acceptor->set_option(asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor->bind(endpoint, error);

	std::cout << "[Info]#LpAcceptor : Bind Success.\n";
}

void LpAcceptor::Listen(int32_t _backLog) {
	m_acceptor->listen(_backLog);

	std::cout << "[Info]#LpAcceptor : Listen.\n";
}

void LpAcceptor::AsyncAccept() {
	LpSession* session = new LpSession(LpIOContext::GetIOContext(), GetIOBufferMaxSize());
	
	m_acceptor->async_accept(*session->GetSocket()
		, std::bind(&LpAcceptor::OnAccept, this, session, std::placeholders::_1));

	std::cout << "[Info]#LpAcceptor : AsyncAccept.\n";
}

void LpAcceptor::OnAccept(LpSession* _session, const system::error_code& _error) {
	if (_error) {
		std::cout << "[Error]#LpAcceptor : OnAccept Fail.\n";
		delete _session;
	}
	else {
		std::cout << "[Info]#LpAcceptor : OnAccept.\n";

		_session->Read();
    }

	AsyncAccept();
}

void LpAcceptor::Close() {

}
}