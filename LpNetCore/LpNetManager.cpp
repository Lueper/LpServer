#include "pch.h"
#include "LpNetManager.h"

namespace lpnet {
LpNetManager::LpNetManager() {
	m_threadCount = 16;
}

LpNetManager::~LpNetManager() {

}

void LpNetManager::Run() {
	m_running = true;

	m_threadVector = std::vector<std::thread*>(m_threadCount);
	for (int i = 0; i < m_threadCount; i++) {
		m_threadVector[i] = new std::thread(&LpNetManager::Process, this);
	}
}

void LpNetManager::Process() {
	while (m_running) {
		NetTask* task = nullptr;
		if (m_netTaskQueue.try_pop(task) == false) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}

		if (task->m_session == nullptr) {
			LpLogger::LOG_DEBUG("Task Session is nullptr");
		}

		switch (task->m_type) {
		case NetTaskType::Receive:
			task->m_session->ProcessReceive();
			break;
		case NetTaskType::Send:
			task->m_session->ProcessSend();
			break;
		default:
			break;
		}
	}
}

void LpNetManager::Stop() {

}

concurrency::concurrent_queue<NetTask*>& LpNetManager::GetNetTaskQueue() {
	return m_netTaskQueue;
}
}