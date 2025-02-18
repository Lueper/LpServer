#include "pch.h"
#include "LpBuffer.h"

namespace lpnet {
//LpBuffer::LpBuffer() : m_maxSize(65536), m_useSize(0), m_lCur(0), m_rCur(0), m_buffer(nullptr), m_spinLock() {
//	m_buffer = new char[m_maxSize];
//	memset(m_buffer, 0, m_maxSize);
//}

LpBuffer::LpBuffer(uint32_t _size) : m_maxSize(_size), m_useSize(0), m_lCur(0), m_rCur(0), m_buffer(nullptr), m_spinLock() {
	m_buffer = new char[m_maxSize];
	memset(m_buffer, 0, m_maxSize);
}

LpBuffer::~LpBuffer() {
	delete[] m_buffer;
}

void LpBuffer::Clear()
{
	m_spinLock.Lock();

	m_useSize = 0;
	m_lCur = 0;
	m_rCur = 0;

	m_spinLock.UnLock();
}

void LpBuffer::Push(char* _data, uint32_t _size) {
	//std::lock_guard<std::mutex> lock(m_mutex);
	m_spinLock.Lock();

	if (_data == nullptr || _size == 0 || _size > m_maxSize) {
		m_spinLock.UnLock();
		return;
	}

	if (_size <= m_maxSize - m_rCur) {
		memcpy_s(m_buffer + m_rCur, _size, _data, _size);

		m_rCur += _size;
	}
	else {
		uint32_t frontSize = m_maxSize - m_rCur;
		uint32_t backSize = _size - frontSize;

		memcpy_s(m_buffer + m_rCur, frontSize, _data, frontSize);
		memcpy_s(m_buffer, backSize, _data + frontSize, backSize);

		m_rCur = backSize;
	}

	m_useSize += _size;

	m_spinLock.UnLock();
}

void LpBuffer::Pop(char* _data, uint32_t _size) {
	//std::lock_guard<std::mutex> lock(m_mutex);
	m_spinLock.Lock();

	if (_data == nullptr || _size == 0 || _size > m_maxSize || _size > m_useSize) {
		m_spinLock.UnLock();
		return;
	}

	if (_size <= m_maxSize - m_lCur) {
		memcpy_s(_data, _size, m_buffer + m_lCur, _size);

		m_lCur += _size;
	}
	else {
		uint32_t frontSize = m_maxSize - m_lCur;
		uint32_t backSize = _size - frontSize;

		memcpy_s(_data, frontSize, m_buffer + m_lCur, frontSize);
		memcpy_s(_data + frontSize, backSize, m_buffer, backSize);

		m_lCur = backSize;
	}

	m_useSize -= _size;

	m_spinLock.UnLock();
}

void LpBuffer::OnPush(uint32_t _size) {
	std::lock_guard<std::mutex> lock(m_mutex);

	if (_size == 0 || _size > m_maxSize) {
		return;
	}

	if (_size <= m_maxSize - m_rCur) {
		m_rCur += _size;
	}
	else {
		uint32_t frontSize = m_maxSize - m_rCur;
		uint32_t backSize = _size - frontSize;

		m_rCur = backSize;
	}

	m_useSize += _size;
}

char* LpBuffer::GetBuffer() {
	if (m_rCur >= m_maxSize) {
		// error!
	}

	return &(m_buffer[m_rCur]);
}

uint32_t LpBuffer::GetAvailableSize() {
	std::lock_guard<std::mutex> lock(m_mutex);
	//m_spinLock.Lock();
	uint32_t size = m_maxSize - m_useSize;
	//m_spinLock.UnLock();
	return size;
}

uint32_t LpBuffer::GetBufferMaxSize() {
	std::lock_guard<std::mutex> lock(m_mutex);
	//m_spinLock.Lock();
	uint32_t size = m_maxSize;
	//m_spinLock.UnLock();
	return size;
}
}