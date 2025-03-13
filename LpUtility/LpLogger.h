#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <time.h>
#include <sys/timeb.h>
#include <iomanip>
#include <thread>
#include <mutex>
#include <concurrent_queue.h>
#include <queue>

#include "LpUtility.h"
#include "LpSingleton.h"

enum class ELogType : uint8_t {
	debug,
	info,
	warn,
	error,
	fatal
};

const std::string LOG_DESC[]{
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL"
};

namespace lpnet {
	using namespace Concurrency;
#define LOG_DEBUG(_msg)	LOG(ELogType::debug, _msg)
#define LOG_INFO(_msg)	LOG(ELogType::info, _msg)
#define LOG_WARN(_msg)	LOG(ELogType::warn, _msg)
#define LOG_ERROR(_msg)	LOG(ELogType::error, _msg)
#define LOG_FATAL(_msg)	LOG(ELogType::fatal, _msg)

class LpLogger : public LpSingleton<LpLogger> {
public:
	LpLogger();

	static void LOG(ELogType _logType, const wchar_t* _msg);
	static void LOG(ELogType _logType, const std::string& _msg);

	static void Update();
private:
	static void PushLog(ELogType _logType, const std::string& _str);
	static void Print(ELogType _logType, const std::string& _str);

	static void SetColor(ELogType _logType);
	static void ResetColor();

	static concurrent_queue<std::pair<ELogType, std::string>> m_logQueue;
	static std::mutex m_mutex;
};
}