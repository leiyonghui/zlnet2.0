#pragma once
#include "IBuffer.h"

namespace net
{
	void __AddCounter(const std::string& name, int32 add);
	void __DelCounter(const std::string& name, int32 add);

	const std::string BufferName = "Buffer";
	const std::string StackName = "StackBuffer";

    class Buffer : public IBuffer
    {
    private:

        uint32 _size;
        uint32 _capacity;
        char* _buff;

    public:
        static const uint32 kInitialSize = 16;
        
        explicit Buffer(uint32 size): _size(0), _capacity(size > 0 ? size : kInitialSize), _buff(new char[_capacity])
        {
#ifdef _MONITOR
            __AddCounter(BufferName, _capacity);
#endif  //_MONITOR
        }

        virtual ~Buffer()
        {
            delete[] _buff;

#ifdef _MONITOR
			__DelCounter(BufferName, _capacity);
#endif  //_MONITOR
        }

        Buffer(const Buffer& buff)
        {
            _capacity = buff._capacity;
            _size = buff._size;
            _buff = new char[_capacity];
            memcpy(_buff, buff._buff, _size);

#ifdef _MONITOR
			__AddCounter(BufferName, _capacity);
#endif  //_MONITOR
        }

		Buffer& operator = (const Buffer& buff)
		{
            if (&buff == this)
                return *this;

            delete[] _buff;

#ifdef _MONITOR
			__AddCounter(BufferName, _capacity);
#endif  //_MONITOR

			_capacity = buff._capacity;
			_size = buff._size;
			_buff = new char[_capacity];
			memcpy(_buff, buff._buff, _size);
		}

        void swap(Buffer& obj)
        {
            uint32 size = _size;
            uint32 capacity = _capacity;
            char* buff = _buff;

            _size = obj._size;
            _capacity = obj._capacity;
            _buff = obj._buff;

            obj._size = size;
            obj._capacity = capacity;
            obj._buff = buff;
        }

        void clear()
        {
            _size = 0;
        }

        int32 capacity()
        {
            return _capacity;
        }

        virtual const char* data() const override
        {
            return _buff;
        }

        virtual uint32 size() const override
        {
            return _size;
        }

        virtual void write(uint32 pos, const char* buff, uint32 size) override
        {
            assert(pos <= _size);

            ensure(pos + size);
            memcpy(_buff + pos, buff, size);
            _size = pos + size;
        }

        virtual char* write(uint32 pos, uint32 size) override
        {
            if (pos > _size)
                return NULL;
            ensure(pos + size);
            if (pos + size > _size)
                _size = pos + size;
            return _buff + pos;
        }

        virtual void read(uint32 pos, char* buff, uint32 size) const override
        {
            assert(pos + size <= _size);

            memcpy(buff, _buff + pos, size);
        }

        virtual const char* read(uint32 pos, uint32 size) const override
        {
            if (pos + size > _size)
                return NULL;
            return (_buff + pos);
        }

        void printString(int32 len)
        {
            int32 sz = _size < 512 ? _size : 512;
            char str[sz];
            memcpy(str, _buff, sz);
            for (int32 i = 0; i < std::min(sz, len); i++)
            {
                std::cout << int16(str[i]);
            }
            std::cout << std::endl;
        }

        void ensure(uint32 size)
        {
            if (_capacity >= size)
            {
                return;
            }

            uint32 new_capacity = _capacity;
            while (new_capacity < size)
            {
                new_capacity = (new_capacity << 1) + 1;
            }

#ifdef _MONITOR
			__DelCounter(BufferName, _capacity);
            __AddCounter(BufferName, new_capacity);
#endif  //_MONITOR

            char* new_buff = new char[new_capacity];
            memset(new_buff, 0, sizeof(char) * new_capacity);
            memcpy(new_buff, _buff, _size);

            delete[] _buff;
            _buff = new_buff;
            _capacity = new_capacity;
        }
    };

    template<uint16 FIXED_SIZE>
    class StackBuffer : public IBuffer
    {
    public:
        explicit StackBuffer() : _ptr(_buf), _size(0), _capacity(FIXED_SIZE)
        {

        }

        StackBuffer(const char* buf, uint32 size) : _ptr(size > FIXED_SIZE ? new char[size] : _buf), _capacity(size > FIXED_SIZE ? size : FIXED_SIZE)
        {
            memcpy(_ptr, buf, size);

#ifdef _MONITOR
			__AddCounter(StackName, _capacity);
#endif  //_MONITOR

        }

        StackBuffer(const StackBuffer& obj)
        {
            if (obj._capacity > FIXED_SIZE)
            {
                _ptr = new char[obj._capacity];

#ifdef _MONITOR
				__AddCounter(StackName, obj._capacity);
#endif  //_MONITOR

            }
            else {
                _ptr = _buf;
            }
			memcpy(_ptr, obj._ptr, obj._size);
			_size = obj._size;
			_capacity = obj._capacity;
        }

		StackBuffer& operator = (const StackBuffer& obj)
		{
			if (_capacity > FIXED_SIZE) {
                delete[] _ptr;

#ifdef _MONITOR
				__AddCounter(StackName, _capacity);
#endif  //_MONITOR

			}
			if (obj._capacity > FIXED_SIZE) {
                _ptr = new char[obj._capacity];

#ifdef _MONITOR
				__AddCounter(StackName, obj._capacity);
#endif  //_MONITOR

			}
			else {
				_ptr = _buf;
			}
			memcpy(_ptr, obj._ptr, obj._size);
			_size = obj._size;
			_capacity = obj._capacity;
			return *this;
		}

        virtual ~StackBuffer()
        {
            if (_capacity > FIXED_SIZE)
            {
                delete[] _ptr;

#ifdef _MONITOR
				__DelCounter(StackName, _capacity);
#endif  //_MONITOR

            }
        }

		void clear()
		{
			_size = 0;
		}

		virtual const char* data() const override
		{
			return _ptr;
		}

		virtual uint32 size() const override
		{
			return _size;
		}

		virtual void write(uint32 pos, const char* data, uint32 size) override
		{
            assert(pos <= _size);
			ensure(pos + size);
			memcpy(_ptr + pos, data, size);
			if (pos + size > _size)
				_size = pos + size;
		}

		//check null and set the memory bytes you got
		virtual char* write(uint32 pos, uint32 size) override
		{
			if (pos > _size)
				return NULL;
			ensure(pos + size);
			if (pos + size > _size)
				_size = pos + size;
			return _ptr + pos;
		}

		virtual void read(uint32 pos, char* data, uint32 size) const override
		{
            assert(pos + size <= _size);
			memcpy(data, _ptr + pos, size);
		}

		//check null
		virtual const char* read(uint32 pos, uint32 size) const override
		{
			if (pos + size > _size)
				return NULL;
			return _ptr + pos;
		}

		void printString(int32 len)
		{
			int32 sz = _size < 512 ? _size : 512;
			char str[sz];
			memcpy(str, _ptr, sz);
			for (int32 i = 0; i < std::min(sz, len); i++)
			{
				std::cout << int16(str[i]);
			}
			std::cout << std::endl;
		}

		void ensure(uint32 size)
		{
			if (_capacity >= size)
				return;

			uint32 new_capacity = (_capacity << 1) + 1;
			while (new_capacity < size) {
				new_capacity = (new_capacity << 1) + 1;
			}

#ifdef _MONITOR
			__AddCounter(StackName, new_capacity);
#endif  //_MONITOR

            char* new_ptr = new char[new_capacity];
			memcpy(new_ptr, _ptr, _size);

			if (_capacity > FIXED_SIZE) {
                delete[] _ptr;

#ifdef _MONITOR
				__DelCounter(StackName, _capacity);
#endif  //_MONITOR

			}
			_ptr = new_ptr;
			_capacity = new_capacity;
		}

    private:
        char _buf[FIXED_SIZE];
        char* _ptr;
        uint32 _size;
        uint32 _capacity;
    };
}