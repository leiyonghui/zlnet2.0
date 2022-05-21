#pragma once
#include "Configs.h"
#include "IStream.h"

namespace network
{
    class CRingBuff : public IStream
    {
        using uint32 = unsigned int;

        CRingBuff(const CRingBuff&) = delete;
        CRingBuff& operator=(const CRingBuff&) = delete;

    public:

        explicit CRingBuff(uint32 capacity);

        ~CRingBuff();

        void write(char* buff, uint32 len) override;

        void read(char* buff, uint32 len) override;

        void write(uint32 len) override;

        SBufferVec* getBufferVec() override;

		void clear() override;

        bool empty() { return _size > 0; }

        uint32 writableBytes() { return _capacity - _size; };

        uint32 readableBytes() { return _size; }

        uint32 size() { return _size; }

        uint32 capacity() { return _capacity; }


    private:
        void ensure(uint32 capacity);

        uint32 _capacity;
        uint32 _front;
        uint32 _end;
        uint32 _size;
        char* _buff;
        SBufferVec* _writerv;
    };
}

