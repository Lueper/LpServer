#pragma once

#include "LpNetCore.h"
#include "LpIOContext.h"

namespace lpnet {
	using namespace boost;
class LpSocket
{
public:
	LpSocket();
	~LpSocket();

	asio::ip::tcp::socket& GetSocket();
private:
	asio::ip::tcp::socket m_socket;
};
}