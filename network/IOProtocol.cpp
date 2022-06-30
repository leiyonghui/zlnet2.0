#include "IOProtocol.h"
#include "Network.h"

namespace net
{
	IOProtocol::IOProtocol(): _protocolType(EPROTO_TCP), _key(0), _group(0), _network(nullptr)
	{
	}

	IOProtocol::~IOProtocol()
	{
		assert(!_key);
	}

	void IOProtocol::onAwake(EPROTOCOL ptype)
	{
		_protocolType = ptype;
		_group = 0;
	}

	void IOProtocol::onRecycle()
	{
		if (_key) 
		{
			_network->pushKey(_key);
			_key = 0;
		}
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