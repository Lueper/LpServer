#include "pch.h"
#include "LpBuffer.h"

namespace lpnet {
LpBuffer::LpBuffer(uint32_t _size) : m_maxSize(_size), m_length(0), m_offset(0), m_buffer(nullptr) {
	m_buffer = new char[m_maxSize];
	memset(m_buffer, 0, m_maxSize);
}

LpBuffer::~LpBuffer() {
	delete[] m_buffer;
}

void LpBuffer::Clear()
{
	m_length = 0;
	m_offset = 0;
}

char* LpBuffer::GetBuffer() {
	if (m_offset >= m_maxSize) {
		// error!
	}

	return &(m_buffer[m_offset]);
}
}