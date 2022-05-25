#include "IOProtocolEx.h"

namespace engine
{
	bool IOProtocol::isAvailable() const
	{
		return _available;
	}
	void IOProtocol::setAvailable()
	{
		_available = true;
	}
	void IOProtocol::unsetAvailable()
	{
		_available = false;
	}
	void IOProtocol::dispatchPacket(Packet* packet)
	{
	}
	void IOProtocol::onListen()
	{

	}

	void IOProtocol::onUnlisten()
	{

	}

	void IOProtocol::onAccept(network::IOProtocolPtr &protocol)
	{

	}

	void IOProtocol::onClose()
	{

	}

	void IOProtocol::onConnect(bool dis)
	{

	}

	void IOProtocol::onDisConnect()
	{

	}
}


