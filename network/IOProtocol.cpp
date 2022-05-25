#include "IOProtocol.h"
#include "Network.h"

namespace network
{
	IOProtocol::IOProtocol(EPROTOCOL type): _protocolType(type), _key(0), _network(nullptr)
	{
		assert(_network);
	}

	IOProtocol::~IOProtocol()
	{
		if (_key)
			_network->pushKey(_key);
	}
}