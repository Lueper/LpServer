#include "pch.h"
#include "LpLogger.h"

namespace lpnet {
std::mutex LpLogger::m_mutex;
concurrent_queue<std::pair<ELogType, std::string>> LpLogger::m_logQueue;

void LpLogger::LOG(ELogType _logType, const wchar_t* _msg) {
	std::wstring ws(_msg);
	LOG(_logType, std::string(ws.begin(), ws.end()));
}

void LpLogger::LOG(ELogType _logType, const std::string& _msg) {
	std::ostringstream os;
	struct _timeb	  _time;
	tm				   t;

	_ftime64_s(&_time);
	localtime_s(&t, &(_time.time));

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

	os << _msg << "\n";

	PushLog(_logType, os.str());
}

void LpLogger::Update() {
	std::pair<ELogType, std::string> log;

	if (m_logQueue.try_pop(log) == false) {
		return;
	}

	Print(log.first, log.second);
}

void LpLogger::PushLog(ELogType _logType, const std::string& _str) {
	m_logQueue.push(std::make_pair(_logType, _str));
}

void LpLogger::Print(ELogType _logType, const std::string& _str) {
	std::lock_guard<std::mutex> lock(m_mutex);

	SetColor(_logType);

	if (!WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), _str.c_str(), static_cast<DWORD>(_str.size()), NULL, NULL)) {
		std::cerr << "Failed to write to console!" << std::endl;
	}

	ResetColor();
}

void LpLogger::SetColor(ELogType _logType) {
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

void LpLogger::ResetColor() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
}