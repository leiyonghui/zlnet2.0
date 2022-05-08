#pragma once
#include <string.h>
#include <assert.h>
#include "Buffer.h"
#include "Configs.h"

namespace network
{
    class BufferReader
    {
    private:
        Buffer* _buf;
        uint32 _pos;

    public:
        explicit BufferReader(Buffer *buff): _buf(buff), _pos(0)
        {

        }

        virtual ~BufferReader()
        {
            
        }

        int32 bytesLeft() const
        {
            return _pos;
        }

        void clear()
        {
            _pos = 0;
        }

        uint8 readUint8()
        {
            const uint8* value = (const uint8*)_buf->read(_pos, sizeof(uint8));
            if (value != NULL)
            {
                _pos += (uint32)sizeof(uint8);
                return *value;
            }
            else
            {
                assert(false);
            }
        }

        uint16 readUint16()
        {
            const uint16* value = (const uint16*)_buf->read(_pos, sizeof(uint16));
            if (value != NULL)
            {
                _pos += (uint32)sizeof(uint16);
                return networkToHost16(*value);
            }
            else
            {
                assert(false);
            }
        }

        uint32 readUint32()
        {
            const uint32* value = (const uint32*)_buf->read(_pos, sizeof(uint32));
            if (value != NULL)
            {
                _pos += (uint32)sizeof(uint32);
                return networkToHost32(*value);
            }
            else
            {
                assert(false);
            }
        }

        uint64 readUint64()
        {
            const uint64* value = (const uint64*)_buf->read(_pos, sizeof(uint64));
            if (value != NULL)
            {
                _pos += (uint32)sizeof(uint64);
                return networkToHost64(*value);
            }
            else
            {
                assert(false);
            }
        }

        int8 readInt8()
        {
            const int8* value = (const int8*)_buf->read(_pos, sizeof(int8));
            if (value != NULL)
            {
                _pos += (uint32)sizeof(int8);
                return *value;
            }
            else
            {
                assert(false);
            }
        }

        int16 readInt16()
        {
            const int16* value = (const int16*)_buf->read(_pos, sizeof(int16));
            if (value != NULL)
            {
                _pos += (uint32)sizeof(int16);
                return networkToHost16(*value);
            }
            else
            {
                assert(false);
            }
        }

        int32 readInt32()
        {
            const int32* value = (const int32*)_buf->read(_pos, sizeof(int32));
            if (value != NULL)
            {
                _pos += (uint32)sizeof(int32);
                return networkToHost32(*value);
            }
            else
            {
                assert(false);
            }
        }

        int64 readInt64()
        {
            const int64* value = (const int64*)_buf->read(_pos, sizeof(int64));
            if (value != NULL)
            {
                _pos += (uint32)sizeof(int64);
                return networkToHost64(*value);
            }
            else
            {
                assert(false);
            }
        }

        float readFloat()
        {
            const float* value = (const float*)_buf->read(_pos, sizeof(float));
            if (value != NULL)
            {
                _pos += (uint32)sizeof(float);
                return networkToHostfloat(*value);
            }
			else
			{
				assert(false);
			}
        }

        double readDouble()
        {
			const double* value = (const double*)_buf->read(_pos, sizeof(double));
			if (value != NULL)
			{
				_pos += (uint32)sizeof(double);
				return networkToHostdouble(*value);
			}
			else
			{
				assert(false);
			}
        }

        uint32 readBytes(char* buf, uint32 size)
        {
            const char* value = _buf->read(_pos, size);
            if (value != NULL) 
            {
                memcpy(buf, value, size);
                _pos += size;
                return size;
            }
            else 
            {
                assert(false);
            }
        }

        char* read(int32 len)
        {
            char* ptr = _buf->read(_pos, len);
            _pos += len;
            return ptr;
        }

        char* read()
        {
            return _buf->read(_pos, 0);
        }

        int32 peekInt32() const
        {
            const int32* value = (const int32*)_buf->read(_pos, sizeof(int32));
            if (value)
            {
                return networkToHost32(*value);
            }
            return 0;
        }

        void toString()
        {
            _buf->toString();
        }
    };
}