#pragma once

namespace lpnet {
enum class SessionState : uint8_t {
	Closed = 0,
	Waiting,
	Connecting,
	Connected
};

enum class NetTaskType : uint8_t {
	None = 0,
	Close,
	Receive,
	Send
};

//enum class NetTaskType : uint8_t {
//	None = 0,
//	Accept,
//	Connect,
//	Read,
//	Receive,
//	Write,
//	Send,
//	Wait
//};
}