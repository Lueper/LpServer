#pragma once

#include "LpUtility.h"

#include <concurrent_queue.h>

namespace lpnet {
	using namespace concurrency;
template <typename T>
class LpPool {
public:
	LpPool() {
		m_poolQueue.clear();
	}
	~LpPool() {
		T* obj = nullptr;
		while (m_poolQueue.try_pop(obj)) {
			delete obj;
		}
		m_poolQueue.clear();
	}

	T* Alloc() {
		T* obj = new T();
		if (obj == nullptr) {
			return nullptr;
		}

		return obj;
	}

	T* Pop() {
		T* obj = nullptr;

		if (m_poolQueue.try_pop(obj) == false) {
			obj = Alloc();
		}

		return obj;
	}

	void Push(T* _obj) {
		m_poolQueue.push(_obj);
	}

	int UseObjectID() {
		if (++m_objectID == INT_MAX) {
			m_objectID = 1;
		}

		return m_objectID;
	}

private:
	concurrent_queue<T*> m_poolQueue;
	std::atomic<int> m_objectID = 0;
};
}