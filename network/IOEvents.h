#pragma once
#include "IOEvent.h"
#include "Address.h"
#include "IOProtocol.h"

namespace network
{
	class IOListen : public IOEvent
	{
	public:
		IOListen(uint16 port, const IOProtocolPtr& protocol) : IOEvent(IO_EVENT_LISTEN, protocol->getKey()), _port(port), _protocol(protocol)
		{
		}

		IOProtocolPtr& getProtocol() { return _protocol; }
		uint16 getPort() const { return _port; };
	private:
		uint16 _port;
		IOProtocolPtr _protocol;
	};
	USING_SHARED_PTR(IOListen);

	template<class UserData>
	class IOEventData : public IOEvent
	{
	public:
		IOEventData(uint32 key) :IOEvent(IO_EVENT_DATA, key)
		{

		}

		UserData getData() { return _data; }
	private:
		UserData _data;
	};
}