#pragma once

#include "LpNetCore.h"

namespace lpnet {
	using namespace boost;
class LpIOContext {
public:
	LpIOContext();
	~LpIOContext();

	void Run();
	void Stop();
	void Restart();

	asio::io_context* GetIOContext();

protected:
	std::atomic<bool> m_running;

private:
	asio::io_context* m_ioContext;
};
}