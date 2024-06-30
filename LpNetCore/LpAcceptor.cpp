#include "pch.h"
#include "LpAcceptor.h"

namespace lpnet {
LpAcceptor::LpAcceptor() : m_acceptor(LpIOContext::Instance().GetIOContext()) {

}

LpAcceptor::~LpAcceptor() {

}

void LpAcceptor::Bind(const std::string ip, uint16_t port) {
	boost::system::error_code error;

	asio::ip::address address = asio::ip::make_address(ip);
	asio::ip::tcp::endpoint endpoint(address, port);
	m_acceptor.open(endpoint.protocol(), error);
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	m_acceptor.bind(endpoint, error);

	std::cout << "[Info] Bind Success.\n";
}

void LpAcceptor::Listen(int32_t backLog) {
	m_acceptor.listen(backLog);

	std::cout << "[Info] Listen.\n";
}

void LpAcceptor::Accept(asio::ip::tcp::socket& socket) {
	m_acceptor.async_accept(socket
		, std::bind(&LpAcceptor::OnAccept, this, std::placeholders::_1));

	std::cout << "[Info] Accept.\n";
}

void LpAcceptor::OnAccept(const system::error_code& error) {
	if (error.value() != 0) {
		std::cout << "Accpet Fail : [value: " << error.value() << "][msg: " << error.message() << "]";

        return;
    }

	std::cout << "[Info] OnAccept.";
	// Read!
}

void LpAcceptor::Close() {

}
}