#include "pch.h"
#include "LpPacketDataPool.h"

void LpPacketDataPool::Alloc() {
	char* data = new LpSession(m_ioContext->GetIOContext());
	if (session == nullptr) {
		return nullptr;
	}

	return session;
}