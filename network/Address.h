#pragma once
#include "Configs.h"

namespace network
{
	class CAddress
	{
	public:
		static const CAddress NONE;
		
		CAddress();

		CAddress(uint32 ip, uint16 port);

		CAddress(std::string ip, uint16 port);

		CAddress(const CAddress& address);

		CAddress(const class sockaddr_in& addr);

		inline  uint32 ip() const { return _ip; }

		inline  uint16 port() const { return _port; }

		inline void ip(uint32 ip) { _ip = ip; }

		inline void port(uint16 port) { _port = port; };

		std::string toString() const;

		static bool String2Ip(const char* string, uint32& address);
		static bool Ip2String(uint32 address, char *string);

#ifdef __linux
		sockaddr_in get_sockaddr_in() const;
#endif // __linux


	private:
		uint32 _ip;
		uint16 _port;
	};

	inline bool operator==(const CAddress& a, const CAddress& b)
	{
		return a.ip() == b.ip() && a.port() == b.port();
	}

	inline bool operator!=(const CAddress& a, const CAddress& b)
	{
		return (a.ip() != b.ip()) || (a.port() != b.port());
	}

	inline bool operator<(const CAddress& a, const CAddress& b)
	{
		return a.ip() < b.ip() || (a.ip() == b.ip() && a.port() < b.port());
	}
}