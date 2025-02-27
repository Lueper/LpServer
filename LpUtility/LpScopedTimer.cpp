#include "pch.h"
#include "LpScopedTimer.h"

namespace lpnet {
void LpScopedTimer::BeginScope() {
	m_startTime = std::chrono::high_resolution_clock::now();
	m_isStarted = true;
}

void LpScopedTimer::EndScope(const std::string& _funcName) {
	if (!m_isStarted) {
		throw std::runtime_error("BeginScope() must be called before EndScope()");
	}

	m_endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = m_endTime - m_startTime;
	
	std::ostringstream msg;
	msg << "#" << _funcName << " Execution time: " << to_string(elapsed.count()) << " seconds";
	LpLogger::LOG_DEBUG(msg.str());
}
}