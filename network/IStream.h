#pragma once

namespace network
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

		virtual void write(char* buff, unsigned int len) = 0;

		virtual void read(char* buff, unsigned int len) = 0;

		virtual void write(unsigned int len) = 0;

		virtual SBufferVec* getBufferVec() = 0;

		virtual void clear() = 0;
	};
}