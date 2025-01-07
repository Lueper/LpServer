#include "pch.h"
#include "LpBuffer.h"

namespace lpnet {
LpBuffer::LpBuffer(uint32_t _size) : m_maxSize(_size), m_length(0), m_offset(0), m_buffer(nullptr), m_spinLock() {
	m_buffer = new char[m_maxSize];
	memset(m_buffer, 0, m_maxSize);
}

LpBuffer::~LpBuffer() {
	delete[] m_buffer;
}

void LpBuffer::Clear()
{
	m_spinLock.Lock();

	m_length = 0;
	m_offset = 0;

	m_spinLock.UnLock();
}

void LpBuffer::Push(char* _data, uint32_t _size) {
	m_spinLock.Lock();

	if (_data == nullptr || _size == 0)
		return;

	if (_size > m_maxSize - m_offset)

	m_spinLock.UnLock();
}

void LpBuffer::Pop(char* _data, uint32_t _size) {
	m_spinLock.Lock();

	if (_data == nullptr || _size == 0)
		return;

	m_spinLock.UnLock();
}

char* LpBuffer::GetBuffer() {
	if (m_offset >= m_maxSize) {
		// error!
	}

	return &(m_buffer[m_offset]);
}

uint32_t LpBuffer::GetBufferMaxSize() {
	return m_maxSize;
}
}