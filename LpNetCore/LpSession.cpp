#include "pch.h"
#include "LpSession.h"

namespace lpnet {
LpSession::LpSession() : m_socket(LpIOContext::Instance().GetIOContext()),
							m_readBuffer(65536), m_writeBuffer(65536) {
	m_recvBuffer = new char[65536];
	m_sendBuffer = new char[65536];
}

LpSession::LpSession(uint32_t _size) : m_socket(LpIOContext::Instance().GetIOContext()),
										m_readBuffer(_size), m_writeBuffer(_size) {
	SetIOBufferSize(_size);
	m_recvBuffer = new char[_size];
	m_sendBuffer = new char[_size];
}

LpSession::~LpSession() {
	delete[] m_recvBuffer;
	delete[] m_sendBuffer;
	m_readBuffer.Clear();
	m_writeBuffer.Clear();
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

    m_socket.async_read_some(asio::mutable_buffer(m_recvBuffer, m_ioBufferSize)
                , std::bind(&LpSession::OnRead, this, std::placeholders::_1, std::placeholders::_2));
				
	std::cout << "[Info]#LpSession : Read." << "\n";
}

void LpSession::OnRead(const system::error_code& _error, uint32_t _size) {
	if (_error.value() != 0) {
		std::cout << "[Error]#LpSession : Accpet Fail - [value: " << _error.value() << "][msg: " << _error.message() << "]";

		Close();
	
		return;
	}

	m_readBuffer.Push(m_recvBuffer, _size);

	// TODO: 다른 곳에서 처리
	char* data = new char[_size];
	m_readBuffer.Pop(data, _size);
	LpPacketHandler::Instance().Process(data, _size);
	delete[] data;

	Read();
}

void LpSession::Write() {
	if (m_socket.is_open() == false)
		return;
	
	m_socket.async_write_some(asio::mutable_buffer(m_sendBuffer, m_ioBufferSize)
				, std::bind(&LpSession::OnWrite, this, std::placeholders::_1, std::placeholders::_2));

	std::cout << "[Info]#LpSession : Write." << "\n";
}

void LpSession::OnWrite(const system::error_code& _error, uint32_t _size) {
	if (_error.value() != 0) {
	    std::cout << "[Error]#LpSession : Accpet Fail - [value: " << _error.value() << "][msg: " << _error.message() << "]";
	
	    return;
	}
	
	m_writeBuffer.Push(m_sendBuffer, _size);

	// TODO: 다른 곳에서 처리
	char* data = nullptr;
	m_writeBuffer.Pop(data, _size);
	LpPacketHandler::Instance().Process(data, _size);
}

void LpSession::Init() {

}

void LpSession::Connect(const std::string _ip, uint16_t _port) {
	auto address = asio::ip::make_address(_ip);
	asio::ip::tcp::endpoint endpoint = asio::ip::tcp::endpoint(address, _port);

	LpIOContext::Instance().GetIOContext();

	m_socket.open(endpoint.protocol());
	m_socket.connect(endpoint);
}

void LpSession::Send(void* _buffer, uint32_t _size, uint32_t& sendSize) {

}

asio::ip::tcp::socket& LpSession::GetSocket() {
	return m_socket;
}
}