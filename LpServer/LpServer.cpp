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
	m_acceptor->SetNetManager(m_netManager);

	m_acceptor->Init();
}

void LpServer::Start() {
    // 클라이언트 연결 대기
    m_acceptor->Listen();

    // 비동기 승인 시작
    m_acceptor->AsyncAccept();

	// 이벤트 수신 시작
	Run();

	// NetTask 처리 시작
	m_netManager->Run();
}

void LpServer::Run() {
	m_running = true;
	
	m_threadCount = std::thread::hardware_concurrency() / 2;

	// asio 이벤트 대기
	for (uint32_t i = 0; i < m_threadCount; i++) {
		std::thread* thread = new std::thread([this] {
			m_acceptor->Run();
		});
		m_asioThreadVector.push_back(thread);
	}

	// I/O Queue 데이터 처리
	//for (int i = 0; i < m_ioThreadCount; i++) {
	for (int i = 0; i < m_threadCount; i++) {
		std::thread* thread = new std::thread(std::bind(&LpServer::ProcessNetTask, this, i));
		m_ioThreadVector.push_back(thread);
	}
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
	delete m_netManager;
	m_netManager = nullptr;
}

void LpServer::ProcessNetTask(int _index) {
	while (m_running) {
		//int _size = sizeof(Packet);

		//if (m_acceptor->GetSessions().empty() == true) {
		//	continue;
		//}

		//for (auto& session : m_acceptor->GetSessions()) {
		//	// TODO: 세션이 닫힌 상태면 지워줘야 함
		//	//if ((*session.second).GetState() == lpnet::SessionState::Closed) {
		//	//	m_acceptor->m_sessionMap.erase((*session.second).GetSessionID());
		//	//	continue;
		//	//}

		//	if ((*session.second).GetReadBuffer()->GetUseSize() > 0) {
		//		char* data = new char[_size];
		//		(*session.second).GetReadBuffer()->Pop(data, _size);
		//		lpnet::LpPacketHandler::Instance()->Process(data, _size);
		//		delete[] data;
		//	}
		//}

		//m_netManager->Pop();
		//LpNetManager::Instance()->Pop();
	}
}