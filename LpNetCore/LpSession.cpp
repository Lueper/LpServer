#include "pch.h"
#include "LpSession.h"

namespace lpnet {
//LpSession::LpSession() : m_socket(LpIOContext::Instance().GetIOContext()),
//							m_readBuffer(65536), m_writeBuffer(65536) {
//	m_recvBuffer = new char[65536];
//	m_sendBuffer = new char[65536];
//}
//
LpSession::LpSession(asio::io_context* _ioContext) : m_ioBufferSize(65536) {
	m_socket = new asio::ip::tcp::socket(*_ioContext);
	m_recvBuffer = new char[m_ioBufferSize];
	m_sendBuffer = new char[m_ioBufferSize];
	memset(m_recvBuffer, 0, m_ioBufferSize);
	memset(m_sendBuffer, 0, m_ioBufferSize);
	m_readBuffer = new LpBuffer(m_ioBufferSize);
	m_writeBuffer = new LpBuffer(m_ioBufferSize);
	m_recvData = new char[m_ioBufferSize];

	SetState(SessionState::Closed);

	m_netManager = new LpNetManager();
	m_packetDataPool = new LpPacketDataPool();
	for (uint32_t i = 0; i < 1000; i++) {
		char* packetData = m_packetDataPool->Alloc();
		m_packetDataPool->Push(packetData);
	}
}

LpSession::LpSession(asio::io_context* _ioContext, uint32_t _size) : m_ioBufferSize(_size) {
	m_socket = new asio::ip::tcp::socket(*_ioContext);
	m_recvBuffer = new char[m_ioBufferSize];
	m_sendBuffer = new char[m_ioBufferSize];
	memset(m_recvBuffer, 0, m_ioBufferSize);
	memset(m_sendBuffer, 0, m_ioBufferSize);
	m_readBuffer = new LpBuffer(m_ioBufferSize);
	m_writeBuffer = new LpBuffer(m_ioBufferSize);
	m_recvData = new char[m_ioBufferSize];
}

LpSession::~LpSession() {
	delete m_socket;
	m_socket = nullptr;
	delete[] m_recvBuffer;
	delete[] m_sendBuffer;
	m_readBuffer->Clear();
	m_writeBuffer->Clear();
	delete m_readBuffer;
	delete m_writeBuffer;
	delete m_recvData;

	delete m_netManager;
	delete m_packetDataPool;
}

void LpSession::Close() {
	if (m_socket->is_open()) {
		m_socket->close();

		//delete m_socket;
		//m_socket = nullptr;

		//LpLogger::LOG_INFO("#LpSession Close");
	}
}

void LpSession::Read() {
	if (m_socket->is_open() == false)
		return;

	m_socket->async_read_some(asio::mutable_buffer(m_recvBuffer, m_ioBufferSize)
		, std::bind(&LpSession::OnRead, this, std::placeholders::_1, std::placeholders::_2));

	//m_socket->async_read_some(asio::mutable_buffer(m_readBuffer->GetBuffer(), m_readBuffer->GetAvailableSize())
	//	, std::bind(&LpSession::OnRead, this, std::placeholders::_1, std::placeholders::_2));
}

void LpSession::OnRead(const system::error_code& _error, uint32_t _size) {
	if (_error.value() != 0) {
		if (_error == asio::error::eof || _error == asio::error::connection_reset) {

		}
		else {
			std::ostringstream errorMsg;
			errorMsg << "#LpSession Read Fail - [value: " << _error.value() << "][msg: " << _error.message() << "]";
			LpLogger::LOG_ERROR(errorMsg.str());
		}

		SetState(SessionState::Closed);
		
		Close();
	
		return;
	}

	m_readBuffer->Push(m_recvBuffer, _size);

	NetTask* netTask = new NetTask(NetTaskType::Receive, this);
	m_netManager->GetNetTaskQueue().push(netTask);

	//m_readBuffer->Push(m_recvBuffer, _size);

	//char* data = new char[_size];
	//m_readBuffer->Pop(data, _size);
	//LpPacketHandler::Instance()->Process(data, _size);
	//delete[] data;

	///////////////////////////////////////////////////////

	//m_readBuffer->OnPush(_size);

	//// TODO: 다른 곳에서 처리
	//uint32_t remainSize = _size;
	//uint32_t packetSize = sizeof(Packet);

	//while (remainSize > 0) {
	//	char* data = new char[packetSize];
	//	m_readBuffer->Pop(data, packetSize);
	//	LpPacketHandler::Instance()->Process(data, remainSize);
	//	delete[] data;

	//	remainSize -= packetSize;
	//}

	Read();
}

void LpSession::ProcessReceive() {
	int _size = sizeof(Packet);

	while (GetReadBuffer()->GetUseSize() > 0) {
		GetReadBuffer()->Pop(m_recvData, _size);
		lpnet::LpPacketHandler::Instance()->Process(m_recvData, _size);
	}
}

void LpSession::ProcessReceive(int& _recvCount) {
	int _size = sizeof(Packet);

	while (GetReadBuffer()->GetUseSize() > 0) {

		char* data = new char[_size];
		//char* data = m_packetDataPool->Pop();

		GetReadBuffer()->Pop(data, _size);
		lpnet::LpPacketHandler::Instance()->Process(data, _size);
		delete data;
		//m_packetDataPool->Push(data);

		_recvCount++;
	}
}

void LpSession::Write(uint32_t _size) {
	if (m_socket->is_open() == false)
		return;

	m_writeBuffer->Pop(m_sendBuffer, _size);

	m_trySendCnt++;
	//std::ostringstream msg;
	//msg << "SendCount : [Try: " << m_trySendCnt << "][Send: " << m_sendCnt << "]";
	//LpLogger::LOG_DEBUG(msg.str());
	
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

	//std::ostringstream msg;
	//msg << "SendCount : [Try: " << m_trySendCnt << "][Send: " << m_sendCnt << "]";
	//LpLogger::LOG_DEBUG(msg.str());

	if (m_trySendCnt >= m_sendCnt) {
		if (m_socket != nullptr && m_socket->is_open() && GetState() == SessionState::Connected) {
			SetState(SessionState::Closed);
		}
	}
}

void LpSession::ProcessSend() {

}

void LpSession::Init() {

}

void LpSession::Reset() {

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