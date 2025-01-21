#include "pch.h"
#include "LpLogger.h"

namespace lpnet {
void LpLogger::LOG(ELogType _logType, const wchar_t* msg) {
	std::wstring ws(msg);
	LOG(_logType, std::string(ws.begin(), ws.end()));
}

void LpLogger::LOG(ELogType _logType, const std::string& msg) {
	std::ostringstream os;
	struct _timeb	  _time;
	tm				   t;
	std::string output;

	_ftime64_s(&_time);
	localtime_s(&t, &(_time.time));
	std::string logLevel = LOG_DESC[(int)_logType];

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

	// [LogType]
	os << std::setfill((' '))
		<< std::setw(5)
		<< std::left
		<< logLevel << ("[");

	// 스레드 번호
	os << "" << ("] ");

	os << msg << std::endl;

	std::cout << os.str();
}
}