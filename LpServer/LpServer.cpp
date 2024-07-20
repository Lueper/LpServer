// 비동기 서버

#include "LpServer.h"

using namespace boost;

LpServer::LpServer() {
    m_acceptor = new lpnet::LpAcceptor();
}

LpServer::LpServer(const std::string _ip, uint16_t _port) {
    m_acceptor = new lpnet::LpAcceptor();
    m_acceptor->Bind(_ip, _port);
}

LpServer::~LpServer() {

}

void LpServer::Run() {
    // 클라이언트 연결 대기
    m_acceptor->Listen();

    // 비동기 승인 시작
    m_acceptor->AsyncAccept();

    // 이벤트가 없을 때까지 대기
    lpnet::LpIOContext::Instance().Run();
}

#pragma region Accept & Close
//void LpServer::Accept() {
//    m_acceptor->async_accept(*m_socket
//                , std::bind(&LpServer::OnAccept, this, std::placeholders::_1));
//}
//
//void LpServer::OnAccept(const system::error_code& _error) {
//    if (_error.value() != 0) {
//        std::cout << "Accpet Fail : [value: " << _error.value() << "][msg: " << _error.message() << "]";
//
//        return;
//    }
//
//    Read();
//}
//
//void LpServer::Close() {
//    if (m_socket->is_open())
//        m_socket->close();
//}
#pragma endregion

#pragma region Read & Write
//void LpServer::Read() {
//    memset(&m_recvBuffer, '\0', sizeof(m_recvBuffer));
//
//    m_socket->async_read_some(asio::buffer(m_recvBuffer, g_bufferSize)
//                , std::bind(&LpServer::OnRead, this, std::placeholders::_1));
//}
//
//void LpServer::OnRead(const system::error_code& _error) {
//    if (_error.value() != 0) {
//        std::cout << "Accpet Fail : [value: " << _error.value() << "][msg: " << _error.message() << "]";
//
//        Close();
//        Accept();
//
//        return;
//    }
//
//    // TODO: 읽기 작업
//    // 
//    ///////////////////
//
//    Read();
//}
//
//void LpServer::Write() {
//    memset(&m_sendBuffer, '\0', sizeof(m_sendBuffer));
//
//    asio::async_write(*m_socket
//            , asio::buffer(m_sendBuffer, g_bufferSize)
//            , std::bind(&LpServer::OnWrite, this, std::placeholders::_1));
//}
//
//void LpServer::OnWrite(const system::error_code& _error) {
//    if (_error.value() != 0) {
//        std::cout << "Accpet Fail : [value: " << _error.value() << "][msg: " << _error.message() << "]";
//
//        return;
//    }
//
//    // TODO: 쓰기 작업
//    // 
//    ///////////////////
//}
#pragma endregion