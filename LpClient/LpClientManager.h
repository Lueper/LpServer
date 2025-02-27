//#pragma once
//
//#include "LpSingleton.h"
//
//namespace lpnet {
//	using namespace std;
//class LpClientManager : LpSingleton<LpClientManager> {
//public:
//	AddSendCount()
//	{ lock_guard<mutex> lock(m_mutex); m_sendCount++; };
//	AddSuccessCount()
//	{ lock_guard<mutex> lock(m_mutex); m_successCount++; };
//	AddTotalSendCount()
//	{ lock_guard<mutex> lock(m_mutex); m_totalSendCount += m_sendCount; m_sendCount = 0; };
//	AddTotalSuccessCount()
//	{ lock_guard<mutex> lock(m_mutex); m_totalSuccessCount += m_successCount; m_successCount = 0; };
//
//	GetSendCount() { return m_sendCount; };
//	GetSuccessCount() { return m_successCount; };
//	GetTotalSendCount() { return m_totalSendCount; };
//	GetTotalSuccessCount() { return m_totalSuccessCount; };
//
//private:
//	uint32_t m_sendCount = 0;
//	uint32_t m_successCount = 0;
//	uint32_t m_totalSendCount = 0;
//	uint32_t m_totalSuccessCount = 0;
//
//	std::mutex m_mutex;
//};
//}