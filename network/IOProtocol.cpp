#include "IOProtocol.h"
#include "Network.h"

namespace network
{
	IOProtocol::IOProtocol(CNetwork* net):_key(0), _network(net)
	{
		assert(_network);
	}

	IOProtocol::~IOProtocol()
	{
		if (_key)
			_network->pushKey(_key);
	}
}