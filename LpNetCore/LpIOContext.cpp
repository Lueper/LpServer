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
	m_ioContext.run();
}

void LpIOContext::Stop() {
	m_ioContext.stop();
}

void LpIOContext::Restart() {
	m_ioContext.restart();
}
}