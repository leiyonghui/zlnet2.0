#pragma once
#include "IOEvent.h"
#include "Address.h"
#include "IOProtocol.h"

namespace network
{
	class IOListen : public IOEvent
	{
	public:
		IOListen(uint16 port, const IOProtocolPtr& protocol) : IOEvent(IO_EVENT_LISTEN, protocol->key()), _port(port), _protocol(protocol)
		{

		}

		IOProtocolPtr& getProtocol() { return _protocol; }

	protected:
		uint16 _port;
		IOProtocolPtr _protocol;
	};
	USING_SHARED_PTR(IOListen);
}