// 비동기 서버

#include "LpServer.h"

using namespace boost;

LpServer::LpServer() {
    m_ioService = new asio::io_service();
    m_endpoint = new asio::ip::tcp::endpoint(asio::ip::address_v6::any(), 8080);
    m_socket = new asio::ip::tcp::socket(*m_ioService);
    m_acceptor = new asio::ip::tcp::acceptor(*m_ioService, *m_endpoint);
}

LpServer::~LpServer() {
    Close();

    delete m_ioService;
    delete m_endpoint;
    delete m_socket;
    delete m_acceptor;
    delete m_ioWork;
    delete m_resolver;
}

void LpServer::Run() {
    Accept();

    m_ioService->run();
}

void LpServer::Accept() {
    m_acceptor->async_accept(*m_socket
                , std::bind(&LpServer::OnAccept, this, std::placeholders::_1));
}

void LpServer::OnAccept(const system::error_code& _error) {
    if (_error.value() != 0) {
        std::cout << "Accpet Fail : [value: " << _error.value() << "][msg: " << _error.message() << "]";

        return;
    }

    Read();
}

void LpServer::Close() {
    if (m_socket->is_open())
        m_socket->close();
}

void LpServer::Read() {
    memset(&m_recvBuffer, '\0', sizeof(m_recvBuffer));

    m_socket->async_read_some(asio::buffer(m_recvBuffer, g_bufferSize)
                , std::bind(&LpServer::OnRead, this, std::placeholders::_1));
}

void LpServer::OnRead(const system::error_code& _error) {
    if (_error.value() != 0) {
        std::cout << "Accpet Fail : [value: " << _error.value() << "][msg: " << _error.message() << "]";

        Close();
        Accept();

        return;
    }

    // TODO: 읽기 작업
    // 
    ///////////////////

    Read();
}

void LpServer::Write() {
    memset(&m_sendBuffer, '\0', sizeof(m_sendBuffer));

    asio::async_write(*m_socket
            , asio::buffer(m_sendBuffer, g_bufferSize)
            , std::bind(&LpServer::OnWrite, this, std::placeholders::_1));
}

void LpServer::OnWrite(const system::error_code& _error) {
    if (_error.value() != 0) {
        std::cout << "Accpet Fail : [value: " << _error.value() << "][msg: " << _error.message() << "]";

        return;
    }

    // TODO: 쓰기 작업
    // 
    ///////////////////
}