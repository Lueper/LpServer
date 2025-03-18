#pragma once

#include "LpNetCore.h"

namespace lpnet {
	using namespace Concurrency;
class LpAcceptor;
class LpSession;
struct NetTask {
	NetTask(NetTaskType _type, LpSession* _session) : m_type(_type), m_session(_session) { }
	void Reset() {
		m_type = NetTaskType::None;
		m_session = nullptr;
	}
	
	NetTaskType m_type = NetTaskType::None;
	LpSession* m_session = nullptr;
};

class LpNetManager {
public:
	LpNetManager();
	~LpNetManager();

	void Run();
	void Process(int _index);
	void Stop();

	void SetThreadConunt(int _count) { m_threadCount = _count; };
	void SetIOThreadConunt(int _count) { m_ioThreadCount = _count; };
	void SetAcceptor(LpAcceptor* _acceptor) { m_acceptor = _acceptor; };
	concurrent_queue<NetTask*>& GetNetTaskQueue(int _index);
private:
	void AddSessionRecvCount(int _sessionID, int _recvCount, int& _totalCount);

	std::vector<concurrent_queue<NetTask*>> m_netTaskQueue;
	std::vector<std::thread*> m_threadVector;
	concurrent_unordered_map<int, int> m_sessionRecvCountMap;

	LpAcceptor* m_acceptor;

	int m_threadCount;
	int m_ioThreadCount;
	std::atomic<int> m_totalCount = 0;

	std::atomic<bool> m_running;
	std::mutex m_mutex;
};
}