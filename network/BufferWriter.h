#pragma once
#include "Buffer.h"

namespace network
{
    class BufferWriter
    {
    private:
        Buffer* _buf;
        uint32 _pos;

    public:
        explicit BufferWriter(Buffer* buff) : _buf(buff), _pos(0)
        {

        }

        virtual ~BufferWriter()
        {
            
        }

        void shrink(uint32 size)
        {
            assert(size <= _pos);
            _pos -= size;
        }

        uint32 bytesWritten() const
        {
            return _pos;
        }

        void writeUint8(uint8 value)
        {
            *(uint8*)_buf->write(_pos, sizeof(uint8)) =  value;
            _pos += (uint32)sizeof(uint8);
        }

        void writeUint16(uint16 value)
        {
            *(uint16*)_buf->write(_pos, sizeof(uint16)) = hostToNetwork16(value);
            _pos += (uint32)sizeof(uint16);
        }

        void writeUint32(uint32 value)
        {
            *(uint32*)_buf->write(_pos, sizeof(uint32)) = hostToNetwork32(value);
            _pos += (uint32)sizeof(uint32);
        }

        void writeUint64(uint64 value)
        {
            *(uint64*)_buf->write(_pos, sizeof(uint64)) = hostToNetwork64(value);
            _pos += (uint32)sizeof(uint64);
        }

        void writeInt8(int8 value)
        {
            *(int8*)_buf->write(_pos, sizeof(int8)) = value;
            _pos += (uint32)sizeof(int8);
        }

        void writeInt16(int16 value)
        {
            *(int16*)_buf->write(_pos, sizeof(int16)) = hostToNetwork16(value);
            _pos += (uint32)sizeof(int16);
        }

        void writeInt32(int32 value)
        {
            *(int32*)_buf->write(_pos, sizeof(int32)) = hostToNetwork32(value);
            _pos += (uint32)sizeof(int32);
        }

        void writeInt64(int64 value)
        {
            *(uint64*)_buf->write(_pos, sizeof(int64)) = hostToNetwork64(value);
            _pos += (uint32)sizeof(int64);
        }

        void writeFloat(float value)
        {
            *(float*)_buf->write(_pos, sizeof(float)) = hostToNetworkfloat(value);
            _pos += (uint32)sizeof(float);
        }

        void writeDouble(double value)
        {
            *(double*)_buf->write(_pos, sizeof(double)) = hostToNetwrokdouble(value);
            _pos += (uint32)sizeof(double);
        }

        void writeString(const std::string& value)
        {
            writeUint32((uint32)value.size());
            _buf->write(_pos, value.data(), (uint32)value.size());
            _pos += (uint32)value.size();
        }

        void writeBytes(const char* buf, uint32 size)
        {
            _buf->write(_pos, buf, size);
            _pos += size;
        }

        char* write(uint32 size)
        {
            char* ptr = _buf->write(_pos, size);
            _pos += size;
            return ptr;
        }

        char* write()
        {
            return _buf->write(_pos, 0);
        }

        void toString()
        {
            _buf->toString();
        }
    };

}
