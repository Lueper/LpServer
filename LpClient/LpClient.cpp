
#include <iostream>
#include <unordered_map>

#include "LpClient.h"

namespace lpnet {
LpClient::LpClient() {
	m_acceptor = new lpnet::LpAcceptor();
	m_resolver = new asio::ip::tcp::resolver(*m_acceptor->GetIOContext());
	m_session = new lpnet::LpSession(m_acceptor->GetIOContext(), 65536);
}

LpClient::~LpClient() {
	delete m_resolver;
	delete m_endPoint;
	delete m_acceptor;
	delete m_session;
}

void LpClient::Init(uint32_t _threadCount, uint32_t _sessionCount) {
	SetThreadCount(_threadCount);
	SetSessionCount(_sessionCount);
}

void LpClient::Run() {
	Connect("192.168.21.96", 7777);

	std::thread* thread = new std::thread([this] {
		m_acceptor->Run();
	});
	m_threadVector.push_back(thread);
}

void LpClient::Connect(const std::string _ip, uint16_t _port) {
	m_endPoint = new asio::ip::tcp::endpoint(asio::ip::address::from_string(_ip), _port);

	m_session->GetSocket()->async_connect(*m_endPoint
		, std::bind(&LpClient::OnConnect, this, m_session, std::placeholders::_1));
}

void LpClient::OnConnect(lpnet::LpSession* _session, const system::error_code& _error) {
	if (_error) {
		lpnet::LpLogger::LOG_ERROR("#LpSession OnConnect Fail");
		delete _session;
	}
	else {
		lpnet::LpLogger::LOG_INFO("#LpSession OnConnect");
	}
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

	for (auto& thread : m_threadVector)
	{
		if (thread->joinable())
			thread->join();
	}
	for (auto& thread : m_threadVector)
	{
		delete thread;
	}
	m_threadVector.clear();
}

void LpClient::Release() {

}

void LpClient::TestSend() {
	Packet packet;

	packet.header.type = 101;
	memset(packet.header.checkSum, 0, sizeof(packet.header.checkSum));
	packet.header.size = sizeof(Packet);

	memset(packet.payload, 0, sizeof(packet.payload));
	std::string str = std::string("qwerasdfzxcv");
	memmove_s(packet.payload, sizeof(packet.payload), str.c_str(), sizeof(str));

	packet.tail.value = 255;

	Send(&packet, sizeof(packet));
}
}