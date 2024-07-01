#pragma once

#include "LpNetCore.h"

namespace lpnet {
	using namespace boost;
class LpSession {
public:
	LpSession();
	~LpSession();

	void Read();
	void OnRead();
	void Write();
	void OnWrite();

	asio::ip::tcp::socket& GetSocket();
private:
	asio::ip::tcp::socket m_socket;
};
}