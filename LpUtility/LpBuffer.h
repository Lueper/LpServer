#pragma once

#include "LpUtility.h"

namespace lpnet {
class LpBuffer {
public:
	LpBuffer(uint32_t _size);
	~LpBuffer();

	//LpBuffer(const LpBuffer&) = delete;
	//LpBuffer& operator=(const LpBuffer&) = delete;
	//LpBuffer& operator=(const LpBuffer&&) = delete;

	void Clear();
	void Push(char* _data, uint32_t _size);
	void Pop(char* _data, uint32_t _size);

	char* GetBuffer();
	uint32_t GetBufferMaxSize();

private:
	LpSpinLock m_spinLock;

	const uint32_t	m_maxSize;
	uint32_t		m_useSize;
	uint32_t		m_offset;
	char*			m_buffer;
};
}