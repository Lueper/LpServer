#pragma once

namespace lpnet {
enum class SessionState : uint8_t {
	Closed = 0,
	Waiting,
	Connecting,
	Connected
};

enum class IOType : uint8_t {
	None = 0,
	Read,
	Write
};

enum class NetEvent : uint8_t {
	None = 0,
	Accept,
	Connect,
	Read,
	Write,
	Wait
};
}