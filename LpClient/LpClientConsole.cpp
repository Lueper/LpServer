#include "LpClientConsole.h"

namespace lpnet {
LpClientConsole::LpClientConsole() {
	m_threadCount = 0;
	m_sessionCount = 0;
}

LpClientConsole::~LpClientConsole() {

}

void LpClientConsole::ClientMain() {
	LpClient* lpClient = new LpClient();
	{
		m_ClientVector.push_back(lpClient);
	}
	
	lpClient->Init(m_threadCount, m_sessionCount);
	
	lpClient->Run();
}

void LpClientConsole::InitCommand() {
	std::string command;

	std::cout << "thread count [1]: ";
	getline(std::cin, command);
	m_threadCount = std::stoul(command);

	std::cout << "send count [10]: ";
	getline(std::cin, command);
	m_sendCount = std::stoul(command);

	//std::cout << "socket count [1]: ";
	//getline(std::cin, command);
	/*m_sessionCount = std::stoul(command);*/

	std::cout << "command [start]: ";
}

bool LpClientConsole::ProcessCommand() {
	std::string command;
	getline(std::cin, command);

	if (command.find("start") != std::string::npos) {
		for (int i = 0; i < m_threadCount; i++) {
			std::thread* thread = new std::thread([this] {
				ClientMain();
			});

			m_threadVector.push_back(thread);
		}

		//Run();
	}

	if (command.find("close") != std::string::npos) {
		//Close();
	}

	if (command.find("send") != std::string::npos) {
		for (int i = 0; i < m_sendCount; i++) {
			for (LpClient* client : m_ClientVector) {
				client->TestSend();
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
}