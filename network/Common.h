#pragma once
#include "Configs.h"

namespace net
{
#define  BYTESWAP16(A) (((A&0xff00)>>8) | ((A&0x00ff)<<8))
#define  BYTESWAP32(A) \
	(((A&(0xff00'0000))>>24) | ((A&(0x00ff'0000))>>8) | \
	((A & 0x0000'ff00) << 8) | ((A & 0x0000'00ff) << 24))
#define  BYTESWAP64(A) \
		(((A&(0xff00'0000'0000'0000))>>56) | ((A&(0x00ff'0000'0000'0000))>>40) | \
		(((A&(0x0000'ff00'0000'0000))>>24) | ((A&(0x0000'00ff'0000'0000))>>8) | \
		((A&0x0000'0000'ff00'0000)<<8) | ((A&0x0000'0000'00ff'0000)<<24)) | \
		((A&0x0000'0000'0000'ff00)<<40) | ((A&0x0000'0000'0000'00ff)<<56))

	namespace common
	{
		union IntWithfloat
		{
			uint32 _int;
			float _float;
		};

		union LongWithDouble
		{
			uint64 _long;
			double _double;
		};

		inline int CheckCPUendian()
		{
			union {
				unsigned long int i;
				unsigned char s[4];
			}c;

			c.i = 0x12345678;
			return (0x12 == c.s[0]);
		}

		SOCKET CreateSocket(EPROTOCOL proto);

		uint64 hostToNetwork64(uint64 host64);

		uint32 hostToNetwork32(uint32 host32);

		uint16 hostToNetwork16(uint16 host16);

		int64 networkToHost64(uint64 net64);

		uint32 networkToHost32(uint32 net32);

		uint16 networkToHost16(uint16 net16);

		float hostToNetworkfloat(float f);

		double hostToNetwrokdouble(double d);

		float networkToHostfloat(float f);

		double networkToHostdouble(double d);
	}
}