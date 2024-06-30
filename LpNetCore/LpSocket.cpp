#include "pch.h"
#include "LpSocket.h"

namespace lpnet {
LpSocket::LpSocket() : m_socket(LpIOContext::Instance().GetIOContext()) {

}

LpSocket::~LpSocket() {

}

asio::ip::tcp::socket& LpSocket::GetSocket() {
	return m_socket;
}
}