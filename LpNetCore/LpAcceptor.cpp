#include "pch.h"
#include "LpAcceptor.h"

namespace lpnet {
LpAcceptor::LpAcceptor() {
	m_acceptor = new asio::ip::tcp::acceptor(*LpIOContext::GetIOContext());
	m_sessionPool = new LpSessionPool((LpIOContext*)this);
}

LpAcceptor::~LpAcceptor() {
	delete m_acceptor;
	m_acceptor = nullptr;
	delete m_sessionPool;
	m_sessionPool = nullptr;
}

void LpAcceptor::Init() {
	m_running = true;

	// Session Pool »ý¼º
	for (uint32_t i = 0; i < SESSION_POOL_SIZE; i++) {
		LpSession* session = m_sessionPool->Alloc();
		m_sessionPool->Push(session);
	}
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
	if (m_running == false || m_acceptor == nullptr) {
		return;
	}

	LpSession* session = m_sessionPool->Pop();

	if (session == nullptr) {
		return;
	}

	try {
		m_acceptor->async_accept(*session->GetSocket()
			, std::bind(&LpAcceptor::OnAccept, this, session, std::placeholders::_1));
	}
	catch (...) {
		m_sessionPool->Push(session);
	}
}

void LpAcceptor::OnAccept(LpSession* _session, const system::error_code& _error) {
	if (m_running == false) {
		if (_session->GetSocket()->is_open() == true) {
			_session->GetSocket()->shutdown(asio::socket_base::shutdown_both);
			_session->GetSocket()->close();
		}

		m_sessionPool->Push(_session);
		return;
	}

	if (_error) {
		if (_session->GetSocket()->is_open() == true) {
			_session->GetSocket()->shutdown(asio::socket_base::shutdown_both);
			_session->GetSocket()->close();
		}

		m_sessionPool->Push(_session);

		std::ostringstream os;
		os << "#LpAcceptor OnAccept Fail : " << _error.message();
		LpLogger::LOG_ERROR(os.str());

		return;
	}

	_session->Read();

	LpLogger::LOG_INFO("#LpAcceptor OnAccept");

	if (m_acceptor->is_open() == true) {
		AsyncAccept();
	}
}

void LpAcceptor::Close() {
	m_running = false;
	m_acceptor->close();
}
}