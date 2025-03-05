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
#define LOG_DEBUG(_msg)	LOG(ELogType::debug, _msg)
#define LOG_INFO(_msg)	LOG(ELogType::info, _msg)
#define LOG_WARN(_msg)	LOG(ELogType::warn, _msg)
#define LOG_ERROR(_msg)	LOG(ELogType::error, _msg)
#define LOG_FATAL(_msg)	LOG(ELogType::fatal, _msg)

class LpLogger : LpSingleton<LpLogger> {
public:
	LpLogger();

	static void LOG(ELogType _logType, const wchar_t* _msg);
	static void LOG(ELogType _logType, const std::string& _msg);

	static std::mutex m_mutex;
private:
	static void Print(ELogType _logType, const std::string& _os);

	static void SetColor(ELogType _logType);
	static void ResetColor();
};
}