#include "pch.h"
#include "LpIOContext.h"

namespace lpnet {
asio::io_context& LpIOContext::GetIOContext() {
	return m_ioContext;
}

LpIOContext::LpIOContext() : m_ioContext() {

}

LpIOContext::~LpIOContext() {

}

void LpIOContext::Run() {
	std::cout << "[Info]#LpIOContext : Run.\n";

	m_ioContext.run();
}

void LpIOContext::Stop() {
	std::cout << "[Info]#LpIOContext : Stop.\n";

	m_ioContext.stop();
}

void LpIOContext::Restart() {
	std::cout << "[Info]#LpIOContext : Restart.\n";

	m_ioContext.restart();
}
}