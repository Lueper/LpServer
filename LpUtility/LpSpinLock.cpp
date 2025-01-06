#include "pch.h"
#include "LpSpinLock.h"

namespace lpnet {
void LpSpinLock::Lock() {
	int expected = 0;
	int desired = 1;

	while (m_lock.compare_exchange_strong(expected, desired, std::memory_order_acquire) == false) {
		expected = 0;
	}
}

void LpSpinLock::UnLock() {
	m_lock.store(0, std::memory_order_release);
}
}