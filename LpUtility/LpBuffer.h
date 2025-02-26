#pragma once

#include <mutex>

#include "LpUtility.h"
#include "LpSpinLock.h"

namespace lpnet {
class LpBuffer {
	//using Size = uint32_t;
public:
	/*LpBuffer();*/
	LpBuffer(uint32_t _size);
	~LpBuffer();

	void Clear();
	void Push(char* _data, uint32_t _size);
	void Pop(char* _data, uint32_t _size);
	void OnPush(uint32_t _size);

	char* GetBuffer();
	uint32_t GetUseSize();
	uint32_t GetAvailableSize();
	uint32_t GetBufferMaxSize();
	uint32_t GetOffset();

private:
	LpSpinLock m_spinLock;
	std::mutex m_mutex;

	const uint32_t	m_maxSize;
	uint32_t		m_useSize;
	uint32_t		m_lCur;
	uint32_t		m_rCur;
	char*			m_buffer;
};
}