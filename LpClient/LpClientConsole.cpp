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
		SetThreadCount(config["ThreadCount"].as<uint32_t>());
		SetSessionCount(config["SessionCount"].as<uint32_t>());
		SetSessionPoolSize(config["SessionPoolSize"].as<uint32_t>());
		
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
		std::thread* clientThread = new std::thread([&] {
			std::lock_guard<std::mutex> lock(m_mutex);
			LpClient* lpClient = new LpClient();
			lpClient->Init(m_threadCount, m_sessionCount, m_ioBufferSize, m_sessionPoolSize);
			lpClient->Connect(m_connectServer.first, m_connectServer.second);

			for (uint32_t i = 0; i < 4; i++) {
				std::thread* thread = new std::thread([&] {
					lpClient->Run();
					lpClient->AsyncWait();

				});
				m_asioThreadVector.push_back(thread);
			}

			m_clientThreadVector.push_back(make_pair(clientThread, lpClient));

			std::this_thread::sleep_for(std::chrono::milliseconds(1));

			int i = 0;
			bool flag = true;
			while (i < 1000) {
				i++;
				lpClient->TestSend();

				{
					

					LpClientManager::Instance()->AddTotalSendCount();
					LpClientManager::Instance()->AddTotalSuccessCount();

					std::ostringstream msg;
					msg << "#Send [SendCnt(suc/cnt): " << LpClientManager::Instance()->GetSuccessCount()
						<< "/" << LpClientManager::Instance()->GetSendCount()
						<< "][TotalCnt(suc/cnt): " << LpClientManager::Instance()->GetTotalSuccessCount()
						<< "/" << LpClientManager::Instance()->GetTotalSendCount() << "]";

					LpLogger::LOG_INFO(msg.str());

					LpClientManager::Instance()->ResetSendCount();
					LpClientManager::Instance()->ResetSuccessCount();
				}
			}
		});
	}
}

void LpClientConsole::Run() {
	ClientMain();
}
}