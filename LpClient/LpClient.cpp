
#include <iostream>

#include "LpClient.h"

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

bool LpClient::ProcessCommand() {
	std::string command;
	getline(std::cin, command);

	if (command.find("start") != std::string::npos) {
		Run();
	}

	if (command.find("close") != std::string::npos) {
		Close();
	}

	if (command.find("send") != std::string::npos) {
		for (int i = 0; i < 1000000; i++)
			TestSend();
	}

	if (command.find("exit") != std::string::npos) {
		return false;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	return true;
}

void LpClient::Init() {
	std::string command;

	std::cout << "thread count [1]: ";
	getline(std::cin, command);
	SetThreadCount(std::stoul(command));

	std::cout << "socket count [1]: ";
	getline(std::cin, command);

	SetSessionCount(std::stoul(command));

	std::cout << "command [start]: ";
}

void LpClient::Run() {
	Connect("192.168.20.158", 77777);

	for (uint32_t i = 0; i < m_threadCount; i++) {
		std::thread* thread = new std::thread([this] {
			m_acceptor->Run();
		});
		m_threadVector.push_back(thread);
	}
}

void LpClient::Connect(const std::string _ip, uint16_t _port) {
	m_endPoint = new asio::ip::tcp::endpoint(asio::ip::address::from_string(_ip), _port);

	m_session->GetSocket()->async_connect(*m_endPoint
		, std::bind(&LpClient::OnConnect, this, m_session, std::placeholders::_1));
}

void LpClient::OnConnect(lpnet::LpSession* _session, const system::error_code& _error) {
	if (_error) {
		std::cout << "[Error]#LpClient : OnConnect Fail.\n";
		delete _session;
	}
	else {
		std::cout << "[Info]#LpClient : OnConnect.\n";
	}
}

void LpClient::Send(Packet* _packet, uint32_t _size) {
	char* data = nullptr;
	lpnet::LpPacketHandler::Instance().ProcessSend(_packet, _size, &data);
	m_session->Send(data, _size);
}

void LpClient::Close() {
	m_session->Close();

	std::cout << "[Info]#LpClient : Close.\n";
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

int main() {

	LpClient lpClient;

	lpClient.Init();

	try {
		while (true) {
			if (lpClient.ProcessCommand() == false) {
				lpClient.Stop();
				lpClient.Release();
			}
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << "\n";
	}

	return 0;
}