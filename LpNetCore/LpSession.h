#pragma once

#include "LpNetCore.h"
#include "LpBuffer.h"

namespace lpnet {
	using namespace boost;
class LpSession {
public:
	LpSession();
	LpSession(asio::io_context* _ioContext);
	LpSession(asio::io_context* _ioContext, uint32_t _size);
	~LpSession();

	void Close();
	void Read();
	void OnRead(const system::error_code& _error, uint32_t _size);
	void Write(uint32_t _size);
	void OnWrite(const system::error_code& _error, uint32_t _size);

	void Init();
	void Reset();
	void Connect(const std::string _ip, uint16_t _port);
	void Send(char* _buffer, uint32_t _size);

	asio::ip::tcp::socket* GetSocket();
	void SetIOBufferSize(uint32_t _size) { m_ioBufferSize = _size; };

	bool IsSended() { return m_isSend; };

	void SetSessionID(int _sessionID) { m_sessionID = _sessionID; };
	int GetSessionID() { return m_sessionID; };

	void SetSendCnt(int _cnt) {	m_sendCnt = _cnt; m_trySendCnt = 0; };

	void SetState(SessionState _state) { m_state = _state; };
	SessionState GetState() { return m_state; };

	LpBuffer* GetReadBuffer() { return m_readBuffer; };
private:
	asio::ip::tcp::socket* m_socket;
	asio::ip::tcp::endpoint* m_endpoint;

	char* m_recvBuffer;
	char* m_sendBuffer;
	LpBuffer* m_readBuffer;
	LpBuffer* m_writeBuffer;

	uint32_t m_ioBufferSize;

	std::recursive_mutex m_cMutex;

	std::atomic<bool> m_isSend = false;
	std::atomic<int> m_trySendCnt = 0;
	std::atomic<int> m_sendCnt = 0;

	std::atomic<int> m_writeCnt = 0;

	std::atomic<int> m_sessionID;

	SessionState m_state;
};
}