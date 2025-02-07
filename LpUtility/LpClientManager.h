#pragma once

#include "LpSingleton.h"

namespace lpnet {
	using namespace std;
	class LpClientManager : public LpSingleton<LpClientManager> {
	public:
		void AddSendCount() {
			lock_guard<mutex> lock(m_mutex); m_sendCount++;
		};
		void AddSuccessCount() {
			lock_guard<mutex> lock(m_mutex); m_successCount++;
		};
		void AddTotalSendCount() {
			lock_guard<mutex> lock(m_mutex); m_totalSendCount += m_sendCount;
		};
		void AddTotalSuccessCount() {
			lock_guard<mutex> lock(m_mutex); m_totalSuccessCount += m_successCount;
		};

		void ResetSendCount() {
			m_sendCount = 0;
		};
		void ResetSuccessCount() {
			m_successCount = 0;
		}

		uint32_t GetSendCount() { return m_sendCount; };
		uint32_t GetSuccessCount() { return m_successCount; };
		uint32_t GetTotalSendCount() { return m_totalSendCount; };
		uint32_t GetTotalSuccessCount() { return m_totalSuccessCount; };

	private:
		uint32_t m_sendCount = 0;
		uint32_t m_successCount = 0;
		uint32_t m_totalSendCount = 0;
		uint32_t m_totalSuccessCount = 0;

		std::mutex m_mutex;
	};
}