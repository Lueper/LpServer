#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <time.h>
#include <sys/timeb.h>
#include <iomanip>

#include "LpUtility.h"

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
#define LOG_DEBUG(msg)	LOG(ELogType::debug, msg)
#define LOG_INFO(msg)	LOG(ELogType::info, msg)
#define LOG_WARN(msg)	LOG(ELogType::warn, msg)
#define LOG_ERROR(msg)	LOG(ELogType::error, msg)
#define LOG_FATAL(msg)	LOG(ELogType::fatal, msg)

class LpLogger {
public:
	static void LOG(ELogType _logType, const wchar_t* msg);
	static void LOG(ELogType _logType, const std::string& msg);

private:
	static void setColor(ELogType _logType);
	static void resetColor();
};
}