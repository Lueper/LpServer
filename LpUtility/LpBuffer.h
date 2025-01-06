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
	char* GetBuffer();

private:
	const uint32_t	m_maxSize;
	uint32_t		m_length;
	int				m_offset;
	char*			m_buffer;
};
}