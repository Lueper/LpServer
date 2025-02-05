
#include <iostream>
#include <unordered_map>

#include "LpClient.h"

namespace lpnet {
LpClient::LpClient() {
	m_acceptor = new lpnet::LpAcceptor();
	m_resolver = new asio::ip::tcp::resolver(*m_acceptor->GetIOContext());
	m_endPoint = nullptr;
}

LpClient::~LpClient() {
	delete m_resolver;
	delete m_endPoint;
	delete m_acceptor;
	delete m_session;
}

void LpClient::Init(uint32_t _threadCount, uint32_t _sessionCount, uint32_t _ioBufferSize) {
	SetThreadCount(_threadCount);
	SetSessionCount(_sessionCount);
	SetIOBufferSize(_ioBufferSize);
}

void LpClient::Run() {
	std::thread* thread = new std::thread([this] {
		m_acceptor->Run();
	});
	m_asioThreadVector.push_back(thread);
}

void LpClient::Connect(const std::string _ip, uint16_t _port) {
	m_endPoint = new asio::ip::tcp::endpoint(asio::ip::address::from_string(_ip), _port);

	for (uint32_t i = 0; i < m_sessionCount; i++) {
		lpnet::LpSession* session = new lpnet::LpSession(m_acceptor->GetIOContext(), m_ioBufferSize);

		session->GetSocket()->async_connect(*m_endPoint
			, std::bind(&LpClient::OnConnect, this, session, std::placeholders::_1));

		m_SessionVector.push_back(session);
	}
}

void LpClient::OnConnect(lpnet::LpSession* _session, const system::error_code& _error) {
	if (_error) {
		lpnet::LpLogger::LOG_ERROR("#LpSession OnConnect Fail");
		delete _session;
		return;
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

void LpClient::TestSend() {
	Packet packet;

	packet.header.type = 101;
	memset(packet.header.checkSum, 0, sizeof(packet.header.checkSum));
	packet.header.size = sizeof(Packet);

	memset(packet.payload, 0, sizeof(packet.payload));
	std::string str = std::string("qwerasdfzxcv");
	str.copy(packet.payload, str.size());

	packet.tail.value = 255;

	for (uint32_t i = 0; i < m_sessionCount; i++) {
		if (m_SessionVector.at(i)->GetSocket()->is_open() == false) {
			i--;
			continue;
		}

		char* data = nullptr;
		lpnet::LpPacketHandler::Instance()->ProcessSend(&packet, sizeof(packet), &data);


		// 랜덤 전송 테스트
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(0, 10);
		int randomNumber = dist(gen);

		for (int i = 0; i < randomNumber; i++) {
			m_SessionVector.at(i)->Send(data, sizeof(packet));

			m_sendCount.fetch_add(1);
			std::ostringstream msg;
			msg << "#LpPacketHandler Send : "
				<< "[type:" << (uint8_t)packet.header.type << "]"
				<< "[checkSum:" << packet.header.checkSum << "]"
				<< "[size:" << (uint32_t)packet.header.size << "]"
				<< "[payload:" << packet.payload << "]"
				<< "[tail:" << (uint8_t)packet.tail.value << "]"
				<< " count : " << m_sendCount;

			LpLogger::LOG_INFO(msg.str());
		}
	}
}
}