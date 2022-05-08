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

        virtual const char* buff() const override
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

        virtual char* read(uint32 pos, uint32 size) const override
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

        void shrink(uint32 pos, int32 reserve)
        {
            assert(pos <= _size);

            int32 len = _size - pos;
            Buffer tmp(len + reserve);
            tmp.write(0, _buff + pos, len);

            swap(tmp);
           /*int32 len = _size - pos;
            _size = len;
            memcpy(_buff, _buff + pos, len);
            ensure(len + reserve);*/
        }
    };
}