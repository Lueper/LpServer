#include "pch.h"
#include "LpSession.h"

namespace lpnet {
//LpSession::LpSession() : m_socket(LpIOContext::Instance().GetIOContext()),
//							m_readBuffer(65536), m_writeBuffer(65536) {
//	m_recvBuffer = new char[65536];
//	m_sendBuffer = new char[65536];
//}
//
//LpSession::LpSession(uint32_t _size) : m_socket(LpIOContext::Instance().GetIOContext()),
//										m_readBuffer(_size), m_writeBuffer(_size) {
//	SetIOBufferSize(_size);
//	m_recvBuffer = new char[_size];
//	m_sendBuffer = new char[_size];
//}

LpSession::LpSession(asio::io_context* _ioContext, uint32_t _size) : m_ioBufferSize(_size) {
	m_socket = new asio::ip::tcp::socket(*_ioContext);
	m_recvBuffer = new char[m_ioBufferSize];
	m_sendBuffer = new char[m_ioBufferSize];
	m_readBuffer = new LpBuffer(m_ioBufferSize);
	m_writeBuffer = new LpBuffer(m_ioBufferSize);
}

LpSession::~LpSession() {
	delete m_socket;
	delete[] m_recvBuffer;
	delete[] m_sendBuffer;
	m_readBuffer->Clear();
	m_writeBuffer->Clear();
	delete m_readBuffer;
	delete m_writeBuffer;
}

void LpSession::Close() {
	if (m_socket->is_open()) {
		m_socket->close();

		LpLogger::LOG_INFO("#LpSession Close");
	}
}

void LpSession::Read() {
	if (m_socket->is_open() == false)
		return;

    m_socket->async_read_some(asio::mutable_buffer(m_recvBuffer, m_ioBufferSize)
                , std::bind(&LpSession::OnRead, this, std::placeholders::_1, std::placeholders::_2));
}

void LpSession::OnRead(const system::error_code& _error, uint32_t _size) {
	if (_error.value() != 0) {
		std::ostringstream errorMsg;
		errorMsg << "#LpSession Read Fail - [value: " << _error.value() << "][msg: " << _error.message() << "]";
		LpLogger::LOG_ERROR(errorMsg.str());

		Close();
	
		return;
	}

	m_readBuffer->Push(m_recvBuffer, _size);

	// TODO: 다른 곳에서 처리
	char* data = new char[_size];
	m_readBuffer->Pop(data, _size);
	LpPacketHandler::Instance()->Process(data, _size);
	delete[] data;

	Read();
}

void LpSession::Write(uint32_t _size) {
	if (m_socket->is_open() == false)
		return;

	m_writeBuffer->Pop(m_sendBuffer, _size);
	
	m_socket->async_write_some(asio::mutable_buffer(m_sendBuffer, _size)
				, std::bind(&LpSession::OnWrite, this, std::placeholders::_1, std::placeholders::_2));
}

void LpSession::OnWrite(const system::error_code& _error, uint32_t _size) {
	if (_error.value() != 0) {
		std::ostringstream errorMsg;
		errorMsg << "#LpSession Write Fail - [value: " << _error.value() << "][msg: " << _error.message() << "]";
		LpLogger::LOG_ERROR(errorMsg.str());
	
	    return;
	}
}

void LpSession::Init() {

}

void LpSession::Connect(const std::string _ip, uint16_t _port) {
	auto address = asio::ip::make_address(_ip);
	asio::ip::tcp::endpoint endpoint = asio::ip::tcp::endpoint(address, _port);

	m_socket->open(endpoint.protocol());
	m_socket->connect(endpoint);
}

void LpSession::Send(char* _buffer, uint32_t _size) {
	
	// 넣고
	m_writeBuffer->Push(_buffer, _size);

	Write(_size);
}

asio::ip::tcp::socket* LpSession::GetSocket() {
	return m_socket;
}
}