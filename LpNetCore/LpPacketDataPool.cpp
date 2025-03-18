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

	memset(obj, 0, m_allocSize);

	return obj;
}

char* LpPacketDataPool::Pop() {
	char* obj = nullptr;

	if (m_poolQueue.try_pop(obj) == false) {
		obj = Alloc();
	}

	return obj;
}

void LpPacketDataPool::Push(char* _obj) {
	if (_obj == nullptr) {
		_obj = Alloc();
	}

	m_poolQueue.push(_obj);
}
}