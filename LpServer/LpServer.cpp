// 비동기 서버

#include "LpServer.h"

LpServer::LpServer() {
    m_acceptor = new lpnet::LpAcceptor();
	m_asioThreadVector.clear();
}

LpServer::LpServer(const std::string _ip, uint16_t _port) {
    m_acceptor = new lpnet::LpAcceptor();
    m_acceptor->Bind(_ip, _port);
	m_asioThreadVector.clear();
}

LpServer::~LpServer() {
}

void LpServer::LoadFile(std::string _filePath) {
    try {
#ifdef _DEBUG
		YAML::Node config = YAML::LoadFile(_filePath)["Server"]["Debug"];
#else
		YAML::Node config = YAML::LoadFile(_filePath)["Server"]["Release"];
#endif
		SetThreadCount(config["ThreadCount"].as<uint32_t>());
		SetIOBufferSize(config["IOBufferSize"].as<uint32_t>());
		SetSessionPoolSize(config["SessionPoolSize"].as<uint32_t>());

		lpnet::LpLogger::LOG_INFO("#YAML Load Config file is Success");
    }
    catch (const YAML::BadFile& e) {
        std::cerr << e.msg << std::endl;
    }
    catch (const YAML::ParserException& e) {
        std::cerr << e.msg << std::endl;
    }
}

bool LpServer::ProcessCommand() {
	std::string command;
	getline(std::cin, command);

	if (command.find("exit") != std::string::npos) {
		return false;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	return true;
}

void LpServer::Init() {
	m_acceptor->SetIOBufferMaxSize(m_ioBufferSize);
	m_acceptor->SetSessionPoolSize(m_sessionPoolSize);

	m_acceptor->Init();
}

void LpServer::Start() {
    // 클라이언트 연결 대기
    m_acceptor->Listen();

    // 비동기 승인 시작
    m_acceptor->AsyncAccept();

	m_threadCount = std::thread::hardware_concurrency() / 2;

    // 이벤트가 없을 때까지 대기
    for (uint32_t i = 0; i < m_threadCount; i++) {
		std::thread* thread = new std::thread([this] {
			Run();
		});
		m_asioThreadVector.push_back(thread);
    }
}

void LpServer::Run() {
	m_acceptor->Run();
}

void LpServer::Stop() {
	m_acceptor->Close();
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

void LpServer::Release() {
	delete m_acceptor;
	m_acceptor = nullptr;
}