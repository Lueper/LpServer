#pragma once

#include "LpSingleton.h"

namespace lpnet {
	using namespace std;
class LpClientManager : public LpSingleton<LpClientManager> {
public:
	void AddSendCount() { lock_guard<mutex> lock(m_mutex); m_sendCount++; };
	void AddSendCount(uint32_t _count) { lock_guard<mutex> lock(m_mutex); m_sendCount += _count; };
	void ResetSendCount() { lock_guard<mutex> lock(m_mutex); m_sendCount = 0; };
	uint32_t GetSendCount() { return m_sendCount; };

	void AddTotalCount() { lock_guard<mutex> lock(m_mutex); m_totalCount += m_sendCount; m_sendCount = 0; };
	void AddTotalCount(uint32_t _count) { lock_guard<mutex> lock(m_mutex); m_totalCount += _count; };
	void ResetTotalCount() { lock_guard<mutex> lock(m_mutex); m_totalCount = 0; }
	uint32_t GetTotalCount() { lock_guard<mutex> lock(m_mutex); return m_totalCount; };

private:
	uint32_t m_sendCount = 0;
	uint32_t m_totalCount = 0;

	std::mutex m_mutex;
};
}