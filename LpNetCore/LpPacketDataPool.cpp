#include "pch.h"
#include "LpPacketDataPool.h"

namespace lpnet {
LpPacketDataPool::LpPacketDataPool(int _size) : LpPool(_size) {

}

char* LpPacketDataPool::Alloc() {
	char* obj = nullptr;

	if (m_allocSize > 0) {
		obj = new char[m_allocSize];
	}
	else {
		obj = new char();
	}

	if (obj == nullptr) {
		return nullptr;
	}

	return obj;
}

char* LpPacketDataPool::Pop() {
	char* obj = nullptr;

	if (m_poolQueue.try_pop(obj) == false) {
		obj = Alloc();
	}

	return obj;
}
}