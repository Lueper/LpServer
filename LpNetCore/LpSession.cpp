#include "pch.h"
#include "LpSession.h"

namespace lpnet {
LpSession::LpSession() : m_socket(LpIOContext::Instance().GetIOContext()) {

}

LpSession::~LpSession() {

}

void LpSession::Close() {
	if (m_socket.is_open()) {
		m_socket.close();

		std::cout << "[Info][LpSession] Close." << "\n";
	}
}

void LpSession::Read() {
	// TODO: 읽기 작업
	std::cout << "[Info][LpSession] Read." << "\n";
}

void OnRead() {

}

void Write() {
	// TODO: 쓰기 작업
}

void OnWrite() {

}

asio::ip::tcp::socket& LpSession::GetSocket() {
	return m_socket;
}
}