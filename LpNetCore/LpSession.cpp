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
	// TODO: �б� �۾�
	std::cout << "[Info][LpSession] Read." << "\n";
}

void OnRead() {

}

void Write() {
	// TODO: ���� �۾�
}

void OnWrite() {

}

asio::ip::tcp::socket& LpSession::GetSocket() {
	return m_socket;
}
}