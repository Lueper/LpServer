#pragma once

#include "LpNetCore.h"

#include <concurrent_queue.h>

namespace lpnet {
class LpSessionPool {
public:
	LpSessionPool(LpIOContext* _ioContext);
	~LpSessionPool();
	
	LpSession* Alloc();
	LpSession* Pop();
	void Push(LpSession* _session);

private:
	int UseSessionID();

	std::atomic<int> m_currentSessionID = 0;
	LpIOContext* m_ioContext;

	Concurrency::concurrent_queue<LpSession*> m_queueSessionPool;
};
}