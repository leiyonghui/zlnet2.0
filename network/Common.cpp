#include "Common.h"

#ifdef __linux
#include <sys/socket.h>
#include <arpa/inet.h>
#endif // __linux

namespace net
{
	namespace common
	{
		SOCKET CreateSocket(EPROTOCOL proto)
		{
			SOCKET fd;
			if (proto == EPROTO_TCP)
				fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			else
				fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (fd == INVALID_SOCKET)
				core_log_error("craete socket", errno, proto);
			return fd;
		}
#ifdef __linux
		uint64 hostToNetwork64(uint64 host64)
		{
			return  CheckCPUendian() ? host64 : htobe64(host64);
		}

		 uint32 hostToNetwork32(uint32 host32)
		{
			return htonl(host32);
		}

		uint16 hostToNetwork16(uint16 host16)
		{
			return htons(host16);
		}

		int64 networkToHost64(uint64 net64)
		{
			return CheckCPUendian() ? net64 : be64toh(net64);
		}

		uint32 networkToHost32(uint32 net32)
		{
			return ntohl(net32);
		}

		uint16 networkToHost16(uint16 net16)
		{
			return ntohs(net16);
		}
#else// __Linux
		SOCKET createSocket(EPROTOCOL proto)
		{

		}

		uint64 hostToNetwork64(uint64 host64)
		{
			return CheckCPUendian() ? host64 : BYTESWAP64(host64);//本地大端则与网络相同
		}

		uint32 hostToNetwork32(uint32 host32)
		{
			return CheckCPUendian() ? host32 : BYTESWAP32(host32);
		}

		uint16 hostToNetwork16(uint16 host16)
		{
			return CheckCPUendian() ? host16 : BYTESWAP16(host16);
		}

		int64 networkToHost64(uint64 net64)
		{
			return CheckCPUendian() ? net64 : BYTESWAP64(net64);
		}

		uint32 networkToHost32(uint32 net32)
		{
			return CheckCPUendian() ? net32 : BYTESWAP32(net32);
		}

		uint16 networkToHost16(uint16 net16)
		{
			return CheckCPUendian() ? net16 : BYTESWAP16(net16);
		}
#endif

		float hostToNetworkfloat(float f)
		{
			IntWithfloat c;
			c._float = f;
			c._int = hostToNetwork32(c._int);
			return c._float;
		}

		double hostToNetwrokdouble(double d)
		{
			LongWithDouble c;
			c._double = d;
			c._long = hostToNetwork64(c._long);
			return c._double;
		}

		float networkToHostfloat(float f)
		{
			IntWithfloat c;
			c._float = f;
			c._int = networkToHost32(c._int);
			return c._float;
		}

		double networkToHostdouble(double d)
		{
			LongWithDouble c;
			c._double = d;
			c._long = networkToHost64(c._long);
			return c._double;
		}
	}
}


