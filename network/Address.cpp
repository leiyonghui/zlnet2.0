#include <sstream>
#include "Common.h"
#include "Configs.h"
#include <string.h>

#ifdef __linux
#include <arpa/inet.h>
#endif // __linux

#include "Address.h"


namespace network
{
	CAddress::CAddress():_ip(0),_port(0) {

	}

	CAddress::CAddress(uint32 ip, uint16 port): _ip(hostToNetwork32(ip)), _port(hostToNetwork16(port)) {

	}

	CAddress::CAddress(std::string ip, uint16 port): _ip(0), _port(hostToNetwork16(port)) {
		String2Ip(ip.c_str(), _ip);
	}

	CAddress::CAddress(const CAddress& address): _ip(address._ip), _port(address._port){

	}

	CAddress::CAddress(const sockaddr_in& addr):_ip(addr.sin_addr.s_addr), _port(addr.sin_port){

	}

	std::string CAddress::toString() const
	{
		auto ip = networkToHost32(_ip);
		auto port = networkToHost16(_port);
		std::stringstream  ostr;
		ostr <<int32((uint8)(ip>>24))<< "." << int32((uint8)(ip>>16))<< "." << int32((uint8)(ip>>8))<< "." << int32((uint8)(ip))<<":"<< port;
		return ostr.str();
	}

	bool CAddress::String2Ip(const char* string, uint32& address)
	{
#ifdef __linux
		in_addr trial;
		if (::inet_aton(string, &trial))
		{
			address = *((int32*)((&trial)));
			return true;
		}
#endif
		return false;
	}

	bool CAddress::Ip2String(uint32 address, char* string)
	{
		address = networkToHost32(address);

		int p1, p2, p3, p4;
		p1 = address >> 24;
		p2 = (address & 0xffffff) >> 16;
		p3 = (address & 0xffff) >> 8;
		p4 = (address & 0xff);

		return sprintf(string, "%d.%d.%d.%d", p1, p2, p3, p4) > 0;
	}

#ifdef __linux
	sockaddr_in CAddress::get_sockaddr_in() const
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = _ip;
		addr.sin_port = _port;
		return addr;
	}
#endif // __liunx

}