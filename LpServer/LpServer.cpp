// 비동기 서버

#include "LpServer.h"

LpServer::LpServer() {
    m_acceptor = new lpnet::LpAcceptor();
	m_threadVector.clear();
}

LpServer::LpServer(const std::string _ip, uint16_t _port) {
    m_acceptor = new lpnet::LpAcceptor();
    m_acceptor->Bind(_ip, _port);
	m_threadVector.clear();
}

LpServer::~LpServer() {
    delete m_acceptor;
}

void LpServer::LoadFile(std::string _filePath) {
    try {
#ifdef _DEBUG
        YAML::Node config = YAML::LoadFile(_filePath)["Debug"];
#else
        YAML::Node config = YAML::LoadFile(_filePath)["Release"];
#endif
        SetThreadCount(config["Server"]["ThreadCount"].as<uint32_t>());
        SetIOBufferSize(config["Server"]["IOBufferSize"].as<uint32_t>());

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
}

void LpServer::Start() {
    // 클라이언트 연결 대기
    m_acceptor->Listen();

    // 비동기 승인 시작
    m_acceptor->AsyncAccept();

	int iWorkCnt = std::thread::hardware_concurrency() / 2;

    // 이벤트가 없을 때까지 대기
    for (uint32_t i = 0; i < m_threadCount; i++) {
		std::thread* thread = new std::thread([this] {
			Run();
		});
		m_threadVector.push_back(thread);
    }
}

void LpServer::Run() {
	m_acceptor->Run();
}

void LpServer::Stop() {
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

void LpServer::Release() {
	delete m_acceptor;
}