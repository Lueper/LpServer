#include "pch.h"
#include "LpSession.h"

namespace lpnet {
LpSession::LpSession() : m_socket(LpIOContext::Instance().GetIOContext()),
							m_recvBuffer(MAX_SIZE), m_sendBuffer(MAX_SIZE) {
}

LpSession::~LpSession() {
	m_recvBuffer.Clear();
	m_sendBuffer.Clear();
}

void LpSession::Close() {
	if (m_socket.is_open()) {
		m_socket.close();

		std::cout << "[Info]#LpSession : Close." << "\n";
	}
}

void LpSession::Read() {
	if (m_socket.is_open() == false)
		return;

    m_socket.async_read_some(asio::mutable_buffer(m_recvBuffer.GetBuffer(), 8196)
                , std::bind(&LpSession::OnRead, this, std::placeholders::_1));
				
	std::cout << "[Info]#LpSession : Read." << "\n";
}

void LpSession::OnRead(const system::error_code& _error) {
	if (_error.value() != 0) {
		std::cout << "Accpet Fail : [value: " << _error.value() << "][msg: " << _error.message() << "]";

		Close();
	
		return;
	}
	
	// TODO: 읽기 작업
	//
	///////////////////
	
	Read();
}

void LpSession::Write() {
	if (m_socket.is_open() == false)
		return;
	
	m_socket.async_write_some(asio::mutable_buffer(m_sendBuffer.GetBuffer(), 8196)
				, std::bind(&LpSession::OnWrite, this, std::placeholders::_1));

	std::cout << "[Info]#LpSession : Write." << "\n";
}

void LpSession::OnWrite(const system::error_code& _error) {
	if (_error.value() != 0) {
	    std::cout << "Accpet Fail : [value: " << _error.value() << "][msg: " << _error.message() << "]";
	
	    return;
	}
	
	// TODO: 쓰기 작업
	// 
	///////////////////
}

asio::ip::tcp::socket& LpSession::GetSocket() {
	return m_socket;
}
}