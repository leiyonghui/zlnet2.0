#pragma once

namespace net
{
	struct SBufferVec
	{
		SBufferVec() :buff(nullptr), len(0) {}
		void clear() { buff = nullptr, len = 0; }

		char* buff;
		int len;
	};

	class IStream
	{
	public:
		IStream() = default;
		virtual~ IStream() = default;

		virtual void write(const char* buff, unsigned int len) = 0;
		virtual void read(char* buff, unsigned int len) = 0;
		virtual void write_confirm(unsigned int len) = 0;
		virtual void read_confirm(unsigned int len) = 0;
		virtual SBufferVec* getWriteableVec() = 0;
		virtual SBufferVec* getReadableVec() = 0;
		virtual void clear() = 0;
		virtual bool empty() = 0;
		virtual uint32 size() = 0;
		virtual uint32 readableBytes() = 0;
		virtual uint32 writableBytes() = 0;
	};
}