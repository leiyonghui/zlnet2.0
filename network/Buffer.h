#pragma once
#include "IBuffer.h"

namespace network
{
    class Buffer : IBuffer
    {

        Buffer(const Buffer& buff) = delete;

        Buffer& operator = (const Buffer& buff) = delete;

    private:

        uint32 _size;
        uint32 _capacity;
        char* _buff;

    public:
        static const uint32 kInitialSize = 16;
        
        explicit Buffer(uint32 size): _size(0), _capacity(size > 0 ? size : kInitialSize), _buff(new char[_capacity])
        {

        }

        virtual ~Buffer()
        {
            delete[] _buff;
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

        void toString()
        {
            int32 sz = _size < 512 ? _size : 512;
            char str[sz];
            memcpy(str, _buff, sz);
            printf("%s\n", str);
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
        }

        StackBuffer(const StackBuffer& obj)
        {
            if (obj._capacity > FIXED_SIZE)
            {
                _ptr = new char[obj._capacity];
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
			}
			if (obj._capacity > FIXED_SIZE) {
                _ptr = new char[obj._capacity];
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
            if (_size > FIXED_SIZE)
            {
                delete[] _ptr;
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
            assert(pos + size < _size);
			memcpy(data, _ptr + pos, size);
		}

		//check null
		virtual const char* read(uint32 pos, uint32 size) const override
		{
			if (pos + size > _size)
				return NULL;
			return _ptr + pos;
		}

	private:
		void ensure(uint32 size)
		{
			if (_capacity >= size)
				return;

			uint32 new_capacity = (_capacity << 1) + 1;
			while (new_capacity < size) {
				new_capacity = (new_capacity << 1) + 1;
			}

            char* new_ptr = new char[new_capacity];
			memcpy(new_ptr, _ptr, _size);

			if (_capacity > FIXED_SIZE) {
                delete[] _ptr;
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