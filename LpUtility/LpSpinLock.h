#pragma once

#include "LpUtility.h"

#include <atomic>

namespace lpnet {
class LpSpinLock {
public:
	void Lock();
	void UnLock();

private:
	std::atomic<int> m_lock{};
};
}