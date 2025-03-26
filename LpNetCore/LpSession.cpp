#include "pch.h"
#include "LpSession.h"

namespace lpnet {
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

	// PacketData char 사이즈는 1024
	//m_packetDataPool = new LpPacketDataPool(65536);
	//for (int i = 0; i < 1; i++) {
	//	char* packetData = m_packetDataPool->Alloc();
	//	m_packetDataPool->Push(packetData);
	//}
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
	//delete m_packetDataPool;
}

void LpSession::Close() {
	if (m_socket->is_open()) {
		m_socket->close();
	}
}

void LpSession::Read() {
	if (m_socket->is_open() == false)
		return;

	uint32_t readSize = m_readBuffer->GetAvailableSize();

	m_socket->async_read_some(asio::mutable_buffer(m_recvBuffer, (readSize > 65536) ? 65536 : readSize)
		, std::bind(&LpSession::OnRead, this, std::placeholders::_1, std::placeholders::_2));
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
	m_netManager->GetNetTaskQueue(m_threadIndex).push(new NetTask(NetTaskType::Receive, this));

	Read();
}

void LpSession::ProcessReceive(int& _recvCount) {
	//std::lock_guard<std::mutex> lock(m_mutex);

	// 사용한 사이즈만큼 처리
	int size = m_readBuffer->GetUseSize();

	//char* data = new char[size];
	//char* data = m_packetDataPool->Pop();
	char* data = PopData(size);

	m_readBuffer->Pop(data, size);
	_recvCount = lpnet::LpPacketHandler::Instance()->PushPacket(GetSessionID(), data, size);

	//m_packetDataPool->Push(data);
	//PushData(data);
}

void LpSession::Write(uint32_t _size) {
	if (m_socket->is_open() == false)
		return;

	m_writeBuffer->Pop(m_sendBuffer, _size);

	m_trySendCnt++;
	
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

void LpSession::SetSessionID(int _sessionID, int _threadCount) {
	m_threadIndex = _sessionID % _threadCount;
	m_sessionID = _sessionID;
}

char* LpSession::PopData(uint32_t _size) {
	if (_size > m_ioBufferSize) {
		char* data = (char*)realloc(m_recvData, _size);
		if (data == nullptr) {
			return nullptr;
		}
		else {
			m_ioBufferSize = _size;
			m_recvData = data;
		}
	}

	memset(m_recvData, 0, sizeof(m_recvData));

	return m_recvData;
}

void LpSession::PushData(char* _data) {

}
}