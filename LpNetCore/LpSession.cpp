#include "pch.h"
#include "LpSession.h"

namespace lpnet {
LpSession::LpSession() : m_socket(LpIOContext::Instance().GetIOContext()) {

}

LpSession::~LpSession() {

}

void LpSession::Read() {
	// TODO: �б� �۾�
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