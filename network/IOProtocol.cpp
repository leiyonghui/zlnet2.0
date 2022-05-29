#include "IOProtocol.h"
#include "Network.h"

namespace network
{
	IOProtocol::IOProtocol(EPROTOCOL protocol): _protocolType(protocol), _key(0), _network(nullptr)
	{
	}

	IOProtocol::~IOProtocol()
	{
		if (_key)
			_network->pushKey(_key);
	}

	void IOProtocol::setLoclAddress(const CAddress* address)
	{
		if (address)
		{
			_localAddress = *address;
		}
		else
		{
			_localAddress = CAddress();
		}
	}

	void IOProtocol::setRemoteAddress(const CAddress* address)
	{
		if (address)
		{
			_remoteAddress = *address;
		}
		else
		{
			_remoteAddress = CAddress();
		}
	}
}