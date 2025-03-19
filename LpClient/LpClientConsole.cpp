#include "LpClientConsole.h"

namespace lpnet {
LpClientConsole::LpClientConsole() {
	m_threadCount = 0;
	m_sessionCount = 0;
}

LpClientConsole::~LpClientConsole() {

}

bool LpClientConsole::ProcessCommand() {
	std::string command;
	getline(std::cin, command);

	//if (command.find("start") != std::string::npos) {
	//	for (uint32_t i = 0; i < m_threadCount; i++) {
	//		std::thread* thread = new std::thread([this] {
	//			ClientMain();
	//		});

	//		m_threadVector.push_back(thread);
	//	}

	//	//Run();
	//}

	if (command.find("close") != std::string::npos) {
		//Close();
	}

	if (command.find("send") != std::string::npos) {
		//for (int i = 0; i < 100; i++) {
		//	for (auto& clientThread : m_clientThreadVector) {
		//		clientThread.second->TestSend();
		//		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//	}
		//}
	}

	if (command.find("exit") != std::string::npos) {
		return false;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	return true;
}

void LpClientConsole::LoadFile(std::string _filePath) {
	try {
#ifdef _DEBUG
		YAML::Node config = YAML::LoadFile(_filePath)["Client"]["Debug"];
#else
		YAML::Node config = YAML::LoadFile(_filePath)["Client"]["Release"];
#endif
		SetIOBufferSize(config["IOBufferSize"].as<uint32_t>());
		SetThreadCount(config["ThreadCount"].as<int>());
		SetSessionCount(config["SessionCount"].as<int>());
		SetSessionPoolSize(config["SessionPoolSize"].as<int>());

		SetClientReconnectCount(config["ClientReconnectCount"].as<int>());
		SetSessionSendCount(config["SessionSendCount"].as<int>());
		
		SetServerCount(config["ServerCount"].as<int>());
		SetSendIndex(config["SendIndex"].as<int>());
		for (int i = 0; i < m_serverCount; i++) {
			std::string ip = config["ServerList"][i]["IP"].as<std::string>();
			uint16_t port = config["ServerList"][i]["Port"].as<uint16_t>();

			m_serverList.push_back(make_pair(ip, port));
		}

		m_connectServer = m_serverList.at(m_sendIndex);

		lpnet::LpLogger::LOG_INFO("#YAML Load Config file is Success");
	}
	catch (const YAML::BadFile& e) {
		std::cerr << e.msg << std::endl;
	}
	catch (const YAML::ParserException& e) {
		std::cerr << e.msg << std::endl;
	}
}

void LpClientConsole::ProcessClient(int index) {
	LpClient* lpClient = m_clientVector[index];

	int i = 0;
	//while (m_running) {
	while (i < m_clientReconnectCount) {
		lpClient->Connect(m_connectServer.first, m_connectServer.second);

		lpClient->CheckSessions();

		lpClient->TestSend();

		lpClient->CloseSessions();

		LpClientManager::Instance()->AddTotalCount(lpClient->m_sendCount);
		std::ostringstream msg;
		msg << "#LpClient [" << index << "] Send [Send:" << lpClient->m_sendCount
			<< "][Total:" << LpClientManager::Instance()->GetTotalCount() << "]";
		LpLogger::LOG_DEBUG(msg.str());

		lpClient->m_sendCount = 0;

		i++;

		//std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
}

void LpClientConsole::ProcessLog() {
	while (m_running) {
		lpnet::LpLogger::Update();
	}
}

void LpClientConsole::Run() {
	//ClientMain();

	// Init
	m_running = true;

	// Start
	for (int i = 0; i < m_threadCount; i++) {
		LpClient* lpClient = new LpClient();
		lpClient->Init(m_threadCount, m_sessionCount, m_ioBufferSize, m_sessionPoolSize, m_sessionSendCount);
		lpClient->Run();
		m_clientVector.push_back(lpClient);
	}

	m_clientWorkVector = std::vector<std::thread>(m_threadCount);

	for (int i = 0; i < m_threadCount; i++) {
		m_clientWorkVector[i] = std::thread(&LpClientConsole::ProcessClient, this, i);
	}

	std::thread* logThread = new std::thread(&LpClientConsole::ProcessLog, this);
}

void LpClientConsole::Stop() {


	for (auto& client : m_clientVector) {
		//client->Release();
		delete client;
	}
	m_clientVector.clear();
}
}