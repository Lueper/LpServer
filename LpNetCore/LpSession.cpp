#include "pch.h"
#include "LpSession.h"

namespace lpnet {
LpSession::LpSession() : m_socket(LpIOContext::Instance().GetIOContext()) {

}

LpSession::~LpSession() {

}

void LpSession::Read() {
	// TODO: 읽기 작업
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