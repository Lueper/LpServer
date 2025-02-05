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
		for (int i = 0; i < 100; i++) {
			for (auto& clientThread : m_clientThreadVector) {
				clientThread.second->TestSend();
				//std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
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
		SetThreadCount(config["ThreadCount"].as<uint32_t>());
		SetSessionCount(config["SessionCount"].as<uint32_t>());
		
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

void LpClientConsole::ClientMain() {
	for (uint32_t i = 0; i < m_threadCount; i++) {
		std::thread* thread = new std::thread([&] {
			LpClient* lpClient = new LpClient();
			lpClient->Init(m_threadCount, m_sessionCount, m_ioBufferSize);
			lpClient->Connect(m_connectServer.first, m_connectServer.second);
			lpClient->Run();

			m_clientThreadVector.push_back(make_pair(thread, lpClient));

			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			while (true) {
				lpClient->TestSend();
			}
		});
	}
}

void LpClientConsole::Run() {
	ClientMain();
}
}