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

	LpLogger::LOG_INFO("#LpAcceptor Bind Success");
}

void LpAcceptor::Listen(int32_t _backLog) {
	m_acceptor->listen(_backLog);

	LpLogger::LOG_INFO("#LpAcceptor Listen");
}

void LpAcceptor::AsyncAccept() {
	LpSession* session = new LpSession(LpIOContext::GetIOContext(), GetIOBufferMaxSize());
	
	m_acceptor->async_accept(*session->GetSocket()
		, std::bind(&LpAcceptor::OnAccept, this, session, std::placeholders::_1));

	LpLogger::LOG_INFO("#LpAcceptor AsyncAccept");
}

void LpAcceptor::OnAccept(LpSession* _session, const system::error_code& _error) {
	if (_error) {
		LpLogger::LOG_ERROR("#LpAcceptor OnAccept Fail");
		delete _session;
	}
	else {
		LpLogger::LOG_INFO("#LpAcceptor OnAccept");

		_session->Read();
    }

	AsyncAccept();
}

void LpAcceptor::Close() {

}
}