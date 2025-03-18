// 비동기 서버

#include "LpServer.h"

LpServer::LpServer() {
    m_acceptor = new lpnet::LpAcceptor();
	m_netManager = new lpnet::LpNetManager();
	m_asioThreadVector.clear();
}

LpServer::LpServer(const std::string _ip, uint16_t _port) {
    m_acceptor = new lpnet::LpAcceptor();
	m_netManager = new lpnet::LpNetManager();
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
		SetThreadCount(config["ThreadCount"].as<int>());
		SetIOThreadCount(config["IOThreadCount"].as<int>());
		SetIOBufferSize(config["IOBufferSize"].as<int>());
		SetSessionPoolSize(config["SessionPoolSize"].as<int>());

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
	m_netManager->SetThreadConunt(m_threadCount);
	m_netManager->SetIOThreadConunt(m_ioThreadCount);

	m_acceptor->SetIOBufferMaxSize(m_ioBufferSize);
	m_acceptor->SetSessionPoolSize(m_sessionPoolSize);
	m_acceptor->SetIOThreadConunt(m_ioThreadCount);
	m_acceptor->SetNetManager(m_netManager);

	m_acceptor->Init();

	lpnet::LpPacketHandler::Instance()->SetIOThreadCount(m_ioThreadCount);
	lpnet::LpPacketHandler::Instance()->Init();
}

void LpServer::Start() {
    // 클라이언트 연결 대기
    m_acceptor->Listen();

    // 비동기 승인 시작
    m_acceptor->AsyncAccept();

	// 이벤트 수신 시작
	Run();

	// NetTask 처리 시작
	m_netManager->SetAcceptor(m_acceptor);
	m_netManager->Run();
}

void LpServer::Run() {
	m_running = true;
	
	//m_threadCount = std::thread::hardware_concurrency() / 2;

	// asio 이벤트 대기
	for (int i = 0; i < m_threadCount; i++) {
		std::thread* thread = new std::thread([this] {
			m_acceptor->Run();
		});
		m_asioThreadVector.push_back(thread);
	}

	// Handler 스레드
	for (int i = 0; i < m_ioThreadCount; i++) {
		m_handlerThread.push_back(std::thread(std::bind(&LpServer::ProcessHandler, this, i)));
	}

	// Log 스레드
	m_logThread = std::thread(std::bind(&LpServer::ProcessLog, this));

	// Main 스레드
	m_mainThread = std::thread(std::bind(&LpServer::ProcessServer, this));

	// I/O Queue 데이터 처리
	//for (int i = 0; i < m_ioThreadCount; i++) {
	//for (int i = 0; i < m_threadCount; i++) {
	//	std::thread* thread = new std::thread(std::bind(&LpServer::ProcessNetTask, this, i));
	//	m_ioThreadVector.push_back(thread);
	//}
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

	for (auto& thread : m_handlerThread)
	{
		if (thread.joinable())
			thread.join();
	}
	m_handlerThread.clear();

	if (m_logThread.joinable()) {
		m_logThread.join();
	}

	if (m_mainThread.joinable()) {
		m_mainThread.join();
	}
}

void LpServer::Release() {
	delete m_acceptor;
	m_acceptor = nullptr;
	delete m_netManager;
	m_netManager = nullptr;
}

void LpServer::ProcessHandler(int _index) {
	while (m_running) {
		lpnet::LpPacketHandler::Instance()->Update(_index);
	}
}

void LpServer::ProcessLog() {
	while (m_running) {
		lpnet::LpLogger::Update();
	}
}

void LpServer::ProcessServer() {
	//while (m_running) {
	//	lpnet::LpLogger::Update();
	//	lpnet::LpPacketHandler::Instance()->Update();
	//}
}