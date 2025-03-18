#include "pch.h"
#include "LpNetManager.h"

namespace lpnet {
LpNetManager::LpNetManager() {
	m_threadCount = 0;
}

LpNetManager::~LpNetManager() {
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

void LpNetManager::Run() {
	m_running = true;

	m_threadVector = std::vector<std::thread*>(m_ioThreadCount);
	for (int i = 0; i < m_ioThreadCount; i++) {
		concurrent_queue<NetTask*> netTaskQueue = concurrent_queue<NetTask*>();
		m_netTaskQueue.push_back(netTaskQueue);
		m_threadVector[i] = new std::thread(&LpNetManager::Process, this, i);
	}
}

void LpNetManager::Process(int _index) {
	while (m_running) {
		NetTask* task = nullptr;
		if (m_netTaskQueue[_index].try_pop(task) == false) {
			std::this_thread::sleep_for(std::chrono::microseconds(100));
			continue;
		}

		if (task->m_session == nullptr) {
			LpLogger::LOG_DEBUG("Task Session is nullptr");
		}

		switch (task->m_type) {
		case NetTaskType::Close: {
			//m_acceptor->CloseSession(task->m_session);
		}
		case NetTaskType::Receive: {
			int recvCount = 0;
			int totalCount = 0;

			//LpScopedTimer::Instance()->BeginScope();
			task->m_session->ProcessReceive(recvCount);
			//LpScopedTimer::Instance()->EndScope(__FUNCTION__);

			if (recvCount > 0) {
				AddSessionRecvCount(task->m_session->GetSessionID(), recvCount, totalCount);
				m_totalCount += recvCount;

				// 1. 세션별 Count 출력
				//if (m_totalCount % 10000 == 0) {
				//	std::ostringstream msg;
				//	msg << "#LpSession Receive : [ID: " << task->m_session->GetSessionID()
				//		<< "][Count: " << totalCount
				//		<< "][Total: " << m_totalCount
				//		<< "]";
				//	LpLogger::LOG_DEBUG(msg.str());
				//}

				
			}
			break;
		}
		case NetTaskType::Send: {
			task->m_session->ProcessSend();
			break;
		}
		default:
			break;
		}
	}
}

void LpNetManager::Stop() {

}

concurrency::concurrent_queue<NetTask*>& LpNetManager::GetNetTaskQueue(int _index) {
	return m_netTaskQueue[_index];
}

void LpNetManager::AddSessionRecvCount(int _sessionID, int _recvCount, int& _totalCount) {
	//std::lock_guard<std::mutex> lock(m_mutex);
	auto iter = m_sessionRecvCountMap.find(_sessionID);
	if (iter == m_sessionRecvCountMap.end()) {
		m_sessionRecvCountMap.insert(make_pair(_sessionID, _recvCount));
		_totalCount = _recvCount;
	}
	else {
		iter->second += _recvCount;
		_totalCount = iter->second;
	}
}
}