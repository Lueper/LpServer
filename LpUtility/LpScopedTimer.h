#pragma once

#include <LpUtility.h>

namespace lpnet {
class LpScopedTimer : public LpSingleton<LpScopedTimer> {
public:
	void BeginScope();
	void EndScope(const std::string& _funcName);

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;

	std::string functionName = "";
	bool m_isStarted = false;
};
}