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
	delete m_netManager;
	m_netManager = nullptr;
}

void LpAcceptor::Init() {
	m_running = true;

	// Session Pool 생성
	for (int i = 0; i < SESSION_POOL_SIZE; i++) {
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

	session->SetSessionID(m_sessionPool->UseSessionID(), GetIOThreadCount());
	session->SetState(SessionState::Waiting);
	m_sessionMap.insert(make_pair(session->GetSessionID(), session));
	LpPacketHandler::Instance()->ResetSequence(session->GetSessionID());

	try {
		m_acceptor->async_accept(*session->GetSocket()
			, std::bind(&LpAcceptor::OnAccept, this, session, std::placeholders::_1));
	}
	catch (...) {
		m_sessionPool->Push(session);
	}
}

void LpAcceptor::OnAccept(LpSession* _session, const system::error_code& _error) {
	if (_session == nullptr) {
		return;
	}

	if (m_running == false) {
		if (_session->GetSocket()->is_open() == true) {
			_session->GetSocket()->shutdown(asio::socket_base::shutdown_both);
			_session->GetSocket()->close();
		}

		_session->SetState(SessionState::Closed);
		m_sessionMap.erase(_session->GetSessionID());
		m_sessionPool->Push(_session);

		//m_netManager->GetNetTaskQueue().push(new NetTask(NetTaskType::Close, this));
		return;
	}

	if (_error) {
		if (_session->GetSocket()->is_open() == true) {
			_session->GetSocket()->shutdown(asio::socket_base::shutdown_both);
			_session->GetSocket()->close();
		}

		_session->SetState(SessionState::Closed);
		//m_sessionMap.erase(_session->GetSessionID());

		m_sessionPool->Push(_session);

		//m_netManager->GetNetTaskQueue().push(new NetTask(NetTaskType::Close, this));

		std::ostringstream os;
		os << "#LpAcceptor OnAccept Fail : " << _error.message();
		LpLogger::LOG_ERROR(os.str());

		return;
	}

	_session->SetNetManager(m_netManager);
	_session->SetState(SessionState::Connected);

	// TODO: Process loop로 변경
	_session->Read();

	//LpLogger::LOG_INFO("#LpAcceptor OnAccept");

	if (m_acceptor->is_open() == true) {
		AsyncAccept();
		//Waiting(_session);
	}
}

void LpAcceptor::Waiting(LpSession* _session) {
	if (m_running == false || m_acceptor == nullptr) {
		return;
	}

	if (_session == nullptr) {
		return;
	}

	try {
		m_acceptor->async_accept(*_session->GetSocket()
			, std::bind(&LpAcceptor::OnAccept, this, _session, std::placeholders::_1));
	}
	catch (...) {
		m_sessionPool->Push(_session);
	}
}

void LpAcceptor::Close() {
	m_running = false;
	m_acceptor->close();
}

void LpAcceptor::CloseSession(LpSession* _session) {
	if (_session == nullptr) {
		return;
	}

	auto iter = m_sessionMap.find(_session->GetSessionID());
	if (iter == m_sessionMap.end()) {
		return;
	}

	m_sessionMap.erase(_session->GetSessionID());
}
}