#include "pch.h"
#include "LpIOContext.h"

namespace lpnet {
LpIOContext::LpIOContext() {
	m_ioContext = new asio::io_context();
}

LpIOContext::~LpIOContext() {
	delete m_ioContext;
}

void LpIOContext::Run() {
	LpLogger::LOG_INFO("#LpIOContext Run.");

	m_ioContext->run();
}

void LpIOContext::Stop() {
	LpLogger::LOG_INFO("#LpIOContext Stop.");

	m_ioContext->stop();
}

void LpIOContext::Restart() {
	LpLogger::LOG_INFO("#LpIOContext Restart.");

	m_ioContext->restart();
}

asio::io_context* LpIOContext::GetIOContext() {
	if (m_ioContext == nullptr) {
		m_ioContext = new asio::io_context();
	}

	return m_ioContext;
}
}