#pragma once

#include "LpNetCore.h"

namespace lpnet {
	using namespace boost;
class LpIOContext {
public:
	static LpIOContext& Instance() {
		static LpIOContext instance;
		return instance;
	}

	asio::io_context& GetIOContext();

	void Run();
	void Stop();
	void Restart();

private:
	LpIOContext();
	~LpIOContext();

	asio::io_context m_ioContext;
};
}