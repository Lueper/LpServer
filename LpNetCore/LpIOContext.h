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

private:
	asio::io_context* m_ioContext;
};
}