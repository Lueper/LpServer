#include "pch.h"
#include "LpSessionPool.h"

namespace lpnet {
LpSessionPool::LpSessionPool(LpIOContext* _ioContext) {
	m_ioContext = _ioContext;
}

LpSessionPool::~LpSessionPool() {
	LpSession* session = nullptr;
	while (m_queueSessionPool.try_pop(session)) {
		delete session;
	}
	m_queueSessionPool.clear();
}

LpSession* LpSessionPool::Alloc() {
	LpSession* session = new LpSession(m_ioContext->GetIOContext());
	if (session == nullptr) {
		return nullptr;
	}

	return session;
}

LpSession* LpSessionPool::Pop() {
	LpSession* session = nullptr;

	if (m_queueSessionPool.try_pop(session) == false) {
		session = Alloc();
	}

	return session;
}

void LpSessionPool::Push(LpSession* _session) {
	_session->Reset();
	m_queueSessionPool.push(_session);
}

int LpSessionPool::UseSessionID() {
	if (++m_currentSessionID == INT_MAX) {
		m_currentSessionID = 1;
	}

	return m_currentSessionID;
}
}