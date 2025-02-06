#include "pch.h"
#include "LpLogger.h"

namespace lpnet {
std::mutex LpLogger::m_mutex;

void LpLogger::LOG(ELogType _logType, const wchar_t* msg) {
	std::wstring ws(msg);
	LOG(_logType, std::string(ws.begin(), ws.end()));
}

void LpLogger::LOG(ELogType _logType, const std::string& msg) {
	std::lock_guard<std::mutex> lock(m_mutex);

	std::ostringstream os;
	struct _timeb	  _time;
	tm				   t;

	_ftime64_s(&_time);
	localtime_s(&t, &(_time.time));
	
	setColor(_logType);

	// [yyyy-mm-dd hh:mm:ss.ms]
	os << t.tm_year + 1900 << ("-");

	os << std::setfill(('0'))
		<< std::setw(2)
		<< t.tm_mon + 1 << ("-");

	os << std::setfill(('0'))
		<< std::setw(2)
		<< t.tm_mday << (" ");

	os << std::setfill(('0'))
		<< std::setw(2)
		<< t.tm_hour << (":");

	os << std::setfill(('0'))
		<< std::setw(2)
		<< t.tm_min << (":");

	os << std::setfill(('0'))
		<< std::setw(2)
		<< t.tm_sec << (".");

	os << std::setfill(('0'))
		<< std::setw(3)
		<< _time.millitm << (" ");

	// LogType
	os << std::setfill((' '))
		<< std::setw(6)
		<< std::left
		<< LOG_DESC[(int)_logType] << ("[");

	// ThreadID
	std::thread::id threadID = std::this_thread::get_id();
	os << threadID << ("] ");

	os << msg << std::endl;

	std::cout << os.str();

	resetColor();
}

void LpLogger::setColor(ELogType _logType) {
	switch (_logType) {
	case ELogType::debug:	// cyan
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case ELogType::info:	// white
		break;
	case ELogType::warn:	// yellow
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case ELogType::error:	// red
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case ELogType::fatal:	// background red
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED);
		break;
	default:
		break;
	}
}

void LpLogger::resetColor() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
}