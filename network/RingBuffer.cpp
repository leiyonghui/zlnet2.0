#include "RingBuffer.h"
#include "core/ResourceMonitor.h"

namespace net
{
	const std::string NAME = "RingBuffer";

	CRingBuffer::CRingBuffer(uint32 capacity) :_capacity(capacity), _front(0), _end(0), _size(0), _buff(new char[_capacity]) {

#ifdef _MONITOR
		ResourceMonitor::Instance()->addCounter(NAME, _capacity);
#endif // _MONITOR

	}

	CRingBuffer::~CRingBuffer()
	{
		delete[] _buff;

#ifdef _MONITOR
		ResourceMonitor::Instance()->delCounter(NAME, _capacity);
#endif  //_MONITOR
	}

	void CRingBuffer::write(const char* buff, uint32 len)
	{
		assert(len > 0);
		ensure(_size + len);
		assert(_end == _front ? (_size == 0) : true);
		if (_end >= _front /*|| (_end == _front && !_size)*/) //ensure保证有空间
		{
			uint32 count = _capacity - _end;
			if (len < count)
			{
				memcpy(_buff + _end, buff, len);
				_end += len;
			}
			else
			{
				uint32 remain = len - count;
				memcpy(_buff + _end, buff, count);
				memcpy(_buff, buff + count, remain);
				_end = remain;
			}
		}
		else
		{
			memcpy(_buff + _end, buff, len);
			_end += len;
		}
		_size += len;
	}

	void CRingBuffer::read(char* buff, uint32 len)
	{
		assert(_size >= len && len);

		if (_end > _front)
		{
#ifdef _DEBUG
			assert(_end - _front >= len);
#endif // DEBUG
			memcpy(buff, _buff + _front, len);
			_front += len;
		}
		else
		{
			uint32 count = _capacity - _front;
			if (count > len)
			{
				memcpy(buff, _buff + _front, len);
				_front += len;
			}
			else
			{
				uint32 remain = len - count;
				memcpy(buff, _buff + _front, count);
				memcpy(buff + count, _buff, remain);
				_front = remain;
			}
		}
		_size -= len;
	}

	void CRingBuffer::write_confirm(uint32 len)
	{
		assert(len > 0 && _size + len <= _capacity);
		if (_end >= _front)
		{
			uint32 count = _capacity - _end;
			if (len < count)
				_end += len;
			else
				_end = len - count;
		}
		else
		{
			_end += len;
		}
		_size += len;
	}

	void CRingBuffer::read_confirm(uint32 len)
	{
		assert(_size >= len && len);

		if (_end > _front)
		{
#ifdef _DEBUG
			assert(_end - _front >= len);
#endif // DEBUG
			_front += len;
		}
		else
		{
			uint32 count = _capacity - _front;
			if (count > len)
			{
				_front += len;
			}
			else
			{
				uint32 remain = len - count;
				_front = remain;
			}
		}
		_size -= len;
	}

	void CRingBuffer::look(char* buff, unsigned int len)
	{
		assert(_size >= len && len);

		if (_end > _front)
		{
#ifdef _DEBUG
			assert(_end - _front >= len);
#endif // DEBUG
			memcpy(buff, _buff + _front, len);
		}
		else
		{
			uint32 count = _capacity - _front;
			if (count >= len)
			{
				memcpy(buff, _buff + _front, len);
			}
			else
			{
				uint32 remain = len - count;
				memcpy(buff, _buff + _front, count);
				memcpy(buff + count, _buff, remain);
			}
		}
	}

	SBufferVec* CRingBuffer::getWriteableVec()
	{
		_writerv[0].clear();
		_writerv[1].clear();
		if (_size == _capacity)
		{
			return _writerv;
		}
		if (_end >= _front)
		{
#ifdef _DEBUG
			assert(_size == 0 ? _end == _front : true);
#endif // _DEBUG
			uint32 len1 = _capacity - _end;
			_writerv[0].buff = len1 > 0 ? _buff + _end : nullptr;
			_writerv[0].len = len1 > 0 ? len1 : 0;
			if (_front > 0) 
			{
				uint32 len2 = _front - 1;
				_writerv[1].buff = len2 > 0 ? _buff : nullptr;
				_writerv[1].len = len2 > 0 ? len2 : 0;
			}
		}
		else
		{
			uint32 len1 = _front - _end;
			_writerv[0].buff = _buff + _end;
			_writerv[0].len = len1;
			_writerv[1].clear();
		}
		return _writerv;
	}

	SBufferVec* CRingBuffer::getReadableVec()
	{
		_readerv[0].clear();
		_readerv[1].clear();
		if (_size == 0)
		{
			return _readerv;
		}
		if (_end > _front)
		{
			_readerv[0].buff = _buff + _front;
			_readerv[0].len = _size;
		}
		else
		{
			int32 len1 = _capacity - _front;
			int32 len2 = _size - len1;
			_readerv[0].buff = _buff + _front;
			_readerv[0].len = len1;
			_readerv[1].buff = _buff;
			_readerv[1].len = len2;
		}
		return _readerv;
	}

	void CRingBuffer::clear()
	{
		_front = 0;
		_end = 0;
		_size = 0;
		_writerv->clear();
		_readerv->clear();
	}

	void CRingBuffer::shrink(uint32 newcapacity)
	{
		if (_size > newcapacity || newcapacity > _capacity)
			return;
		resize(newcapacity);
	}

	void CRingBuffer::ensure(uint32 capacity)
	{
		if (_capacity >= capacity)
			return;

		uint32 newcapacity = _capacity;
		while (newcapacity < capacity)
			newcapacity = (newcapacity << 1) + 1;

		resize(newcapacity);
	}

	void CRingBuffer::resize(uint32 newcapacity)
	{
#ifdef _MONITOR
		ResourceMonitor::Instance()->delCounter(NAME, _capacity);
		ResourceMonitor::Instance()->addCounter(NAME, newcapacity);
#endif // _MONITOR

		char* tembuff = new char[newcapacity];
		if (_end <= _front && _size)
		{
			auto len = _capacity - _front;
#ifdef _DEBUG
			assert(_size == len + _end);
#endif // DEBUG

			memcpy(tembuff, _buff + _front, len);
			memcpy(tembuff + len, _buff, _end);
		}
		else
		{
#ifdef _DEBUG
			assert(_size == _end - _front);
#endif // DEBUG
			memcpy(tembuff, _buff + _front, _size);
		}
		delete[] _buff;
		_buff = tembuff;
		_capacity = newcapacity;
		_front = 0;
		_end = _size;

	}

}