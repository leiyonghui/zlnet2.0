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

	class IIOBuffer
	{
	public:
		IIOBuffer() = default;
		virtual~IIOBuffer() = default;

		virtual void write(const char* buff, unsigned int len) = 0;
		virtual void read(char* buff, unsigned int len) = 0;
		virtual void look(char* buff, unsigned int len) = 0;
		virtual SBufferVec* getWriteableVec() = 0;
		virtual void write_confirm(unsigned int len) = 0;
		virtual SBufferVec* getReadableVec() = 0;
		virtual void read_confirm(unsigned int len) = 0;
		virtual void clear() = 0;
		virtual bool empty() = 0;
		virtual unsigned int size() = 0;
		virtual unsigned int readableBytes() = 0;
		virtual unsigned int writableBytes() = 0;
		virtual void shrink(int newcapacity) = 0;
	};
}