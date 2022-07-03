#pragma once
#include "Configs.h"
#include "IIOBuffer.h"

namespace net
{
    class CRingBuffer : public IIOBuffer
    {
        using uint32 = unsigned int;

        CRingBuffer(const CRingBuffer&) = delete;
        CRingBuffer& operator=(const CRingBuffer&) = delete;

    public:

        explicit CRingBuffer(uint32 capacity);

        ~CRingBuffer();

        void write(const char* buff, uint32 len) override;

        void read(char* buff, uint32 len) override;

        void write_confirm(uint32 len) override;

        void read_confirm(uint32 len) override;

		void look(char* buff, unsigned int len) override;

        SBufferVec* getWriteableVec() override;

        SBufferVec* getReadableVec() override;

		void clear() override;

        bool empty() override { return _size == 0; }

        uint32 writableBytes() override { return _capacity - _size; };

        uint32 readableBytes() override { return _size; }

        uint32 size() override { return _size; }

        uint32 capacity() { return _capacity; }

		void tryshrink();

    private:
        void ensure(uint32 capacity);

		void resize(uint32 newcapacity);

        uint32 _capacity;
        uint32 _front;
        uint32 _end;
        uint32 _size;
        char* _buff;
		SBufferVec _writerv[2];
		SBufferVec _readerv[2];
    };
}

