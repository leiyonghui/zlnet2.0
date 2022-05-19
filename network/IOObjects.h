#pragma once
#include "IOObject.h"

namespace network
{
	class TcpListener : public IOObject
	{
	public:
		TcpListener(uint32 key, IOProtocolPtr protocol) :
			IOObject(IO_OBJECT_TYPE_LISTENER, key, protocol)
		{

		}

		CEndPointUnPtr listen();
	};

	class TcpConnection : public IOObject
	{
	public:
		TcpConnection(uint32 key, IOProtocolPtr protocol) :
			IOObject(IO_OBJECT_TYPE_CONNECTION, key, protocol)
		{

		}
	};

	class TcpConnector : public IOObject
	{
	public:
		TcpConnector(uint32 key, IOProtocolPtr protocol) :
			IOObject(IO_OBJECT_TYPE_CONNECTOR, key, protocol),_state(EDisconnected)
		{

		}
	protected:
		EConnectState _state;
	};
}