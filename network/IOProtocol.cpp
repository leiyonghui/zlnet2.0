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

	void IOProtocol::setLoclAddress(const CAddress& address)
	{
		_localAddress = address;
	}

	void IOProtocol::setRemoteAddress(const CAddress& address)
	{
		_remoteAddress = address;
	}
}