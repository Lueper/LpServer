#pragma once

#include "LpNetCore.h"

namespace lpnet {
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
	void Process();
	void Stop();

	concurrency::concurrent_queue<NetTask*>& GetNetTaskQueue();
private:
	concurrency::concurrent_queue<NetTask*> m_netTaskQueue;
	std::vector<std::thread*> m_threadVector;

	int m_threadCount;

	std::atomic<bool> m_running;

};
}