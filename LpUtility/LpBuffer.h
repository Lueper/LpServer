#pragma once

#include "LpUtility.h"
#include "LpSpinLock.h"

namespace lpnet {
class LpBuffer {
	using Size = uint32_t;
public:
	LpBuffer(Size _size);
	~LpBuffer();

	void Clear();
	void Push(char* _data, Size _size);
	void Pop(char* _data, Size _size);

	char* GetBuffer();
	uint32_t GetAvailableSize();
	uint32_t GetBufferMaxSize();

private:
	LpSpinLock m_spinLock;

	const Size	m_maxSize;
	Size		m_useSize;
	Size		m_lCur;
	Size		m_rCur;
	char*		m_buffer;
};
}