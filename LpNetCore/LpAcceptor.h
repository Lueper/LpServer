#pragma once

#include "LpNetCore.h"


namespace lpnet {
	using namespace boost;
class LpAcceptor : public LpIOContext {
public:
	LpAcceptor();
	~LpAcceptor();

	void Init();

	void Bind(const std::string _ip, uint16_t _port);
	void Listen(int32_t _backLog = asio::ip::tcp::acceptor::max_listen_connections);
	void AsyncAccept();
	void OnAccept(LpSession* _session, const system::error_code& _error);
	void Close();
	void Waiting(LpSession* _session);
	void CloseSession(LpSession* _session);

	void SetIOThreadConunt(int _ioThreadCount) { m_ioThreadCount = _ioThreadCount; };
	int GetIOThreadCount() { return m_ioThreadCount; };
	void SetIOBufferMaxSize(uint32_t _size) { BUFFER_MAX_SIZE = _size; };
	uint32_t GetIOBufferMaxSize() { return BUFFER_MAX_SIZE;	};
	void SetSessionPoolSize(int _size) { SESSION_POOL_SIZE = _size; };

	std::unordered_map<int, LpSession*> GetSessions() { return m_sessionMap; };

	std::unordered_map<int, LpSession*> m_sessionMap;

	void SetNetManager(LpNetManager* _manager) { m_netManager = _manager; };
	LpNetManager* GetNetManager() {	return m_netManager; };

private:
	asio::ip::tcp::acceptor* m_acceptor;
	LpSessionPool* m_sessionPool;
	LpNetManager* m_netManager;

	//std::unordered_map<int, LpSession*> m_sessionMap;

	uint32_t BUFFER_MAX_SIZE = 65536;
	int SESSION_POOL_SIZE = 1000;
	int m_ioThreadCount = 0;
};
}