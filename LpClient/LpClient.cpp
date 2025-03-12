
#include <iostream>
#include <unordered_map>

#include "LpClient.h"

namespace lpnet {
LpClient::LpClient() {
	m_acceptor = new lpnet::LpAcceptor();
	m_resolver = new asio::ip::tcp::resolver(*m_acceptor->GetIOContext());
	m_sessionPool = new LpSessionPool((LpIOContext*)m_acceptor);
	m_endPoint = nullptr;
	m_timer = nullptr;

	m_work = new asio::io_context::work(*m_acceptor->GetIOContext());
}

LpClient::~LpClient() {
	delete m_resolver;
	delete m_endPoint;
	delete m_acceptor;
	delete m_session;
	delete m_sessionPool;
	delete m_timer;
}

void LpClient::Init(uint32_t _threadCount, uint32_t _sessionCount, uint32_t _ioBufferSize, uint32_t _sessionPoolSize, int _sessionSendCount) {
	SetThreadCount(_threadCount);
	SetSessionCount(_sessionCount);
	SetIOBufferSize(_ioBufferSize);
	SetSessionPoolSize(_sessionPoolSize);
	SetSessionSendCount(_sessionSendCount);

	// Session Pool 생성
	for (int i = 0; i < m_sessionPoolSize; i++) {
		LpSession* session = m_sessionPool->Alloc();
		m_sessionPool->Push(session);
	}
}

void LpClient::Run() {
	std::thread* thread = new std::thread([this] {
		m_acceptor->Run();
	});
	m_asioThreadVector.push_back(thread);
}

void LpClient::Connect(const std::string _ip, uint16_t _port) {
	m_endPoint = new asio::ip::tcp::endpoint(asio::ip::address::from_string(_ip), _port);

	for (int i = 0; i < m_sessionCount; i++) {
		lpnet::LpSession* session = m_sessionPool->Pop();

		session->GetSocket()->async_connect(*m_endPoint
			, std::bind(&LpClient::OnConnect, this, session, std::placeholders::_1));

		session->SetState(SessionState::Connecting);

		m_sessionMap.emplace(m_sessionPool->UseSessionID(), session);
	} 
}

void LpClient::OnConnect(lpnet::LpSession* _session, const system::error_code& _error) {
	if (_error) {
		if (m_connectTryCount >= 10) {
			lpnet::LpLogger::LOG_ERROR("#LpSession OnConnect Fail : Connection TryCount Exceeded");

			m_connectTryCount = 0;

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		if (_session->GetState() == SessionState::Connecting) {
			_session->GetSocket()->async_connect(*m_endPoint
				, std::bind(&LpClient::OnConnect, this, _session, std::placeholders::_1));
		}

		m_connectTryCount++;

		return;
	}
	else {
		//lpnet::LpLogger::LOG_INFO("#LpSession OnConnect");

		m_connectTryCount = 0;
	}

	_session->SetState(SessionState::Connected);
}

void LpClient::Send(Packet* _packet, uint32_t _size) {
	char* data = nullptr;
	lpnet::LpPacketHandler::Instance()->ProcessSend(_packet, _size, &data);
	m_session->Send(data, _size);
}

void LpClient::Close() {
	m_session->Close();

	lpnet::LpLogger::LOG_INFO("#LpSession Close");
}

void LpClient::Stop() {
	m_acceptor->Stop();

	for (auto& thread : m_asioThreadVector)
	{
		if (thread->joinable())
			thread->join();
	}
	for (auto& thread : m_asioThreadVector)
	{
		delete thread;
	}
	m_asioThreadVector.clear();
}

void LpClient::Release() {

}

void LpClient::AsyncWait() {
	if (m_timer == nullptr)
	{
		m_timer = new asio::steady_timer(*m_acceptor->GetIOContext());
	}

	// 5초 후, OnWait 호출
	m_timer->async_wait(std::bind(&LpClient::OnWait, this, std::placeholders::_1));
	m_timer->expires_from_now(std::chrono::seconds(5));
}

void LpClient::OnWait(const system::error_code& _error) {
	lpnet::LpLogger::LOG_INFO("#LpClient OnWait");
}

void LpClient::CloseSessions() {
	bool isAllClosed = false;

	while (!isAllClosed) {
		int closeCount = 0;
		for (auto& session : m_sessionMap) {
			if (session.second->GetState() == SessionState::Closed) {
				session.second->Close();
				m_sessionPool->Push(session.second);
				closeCount++;
			}

			if (closeCount == m_sessionMap.size()) {
				isAllClosed = true;
				break;
			}
		}
	}

	m_sessionMap.clear();

	lpnet::LpLogger::LOG_INFO("#LpSession Close All Sessions");
}

void LpClient::CheckSessions() {
	bool isAllConnected = false;

	while (!isAllConnected) {
		int connectedCount = 0;
		for (auto& session : m_sessionMap) {
			if (session.second->GetState() == SessionState::Connected) {
				connectedCount++;
			}

			if (connectedCount == m_sessionMap.size()) {
				isAllConnected = true;
				break;
			}
		}
	}
}

void LpClient::TestSend() {
	for (auto& iter : m_sessionMap) {
		Packet packet;

		packet.header.seqNum = 0;
		packet.header.type = 101;
		memset(packet.header.checkSum, 0, sizeof(packet.header.checkSum));
		packet.header.size = sizeof(Packet);

		memset(packet.payload, 0, sizeof(packet.payload));
		//std::string str = to_string(LpClientManager::Instance()->GetTotalCount() + 1);
		std::string str = std::string("asdfasdfasdfasdfsadfasddfcasdf");
		str.copy(packet.payload, 128);

		//packet.tail.value = 97;
		packet.tail.value = 255;

		LpSession* session = iter.second;

		// 랜덤 전송 테스트
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(1, 10);
		//int randomNumber = dist(gen);
		//int randomNumber = 10;
		int randomNumber = m_sessionSendCount;
		
		session->SetSendCnt(randomNumber);

		for (int i = 0; i < randomNumber; i++) {
			char* data = nullptr;
			lpnet::LpPacketHandler::Instance()->ProcessSend(&packet, sizeof(packet), &data);
			session->Send(data, sizeof(packet));

			m_sendCount++;
			packet.header.seqNum++;

			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
}