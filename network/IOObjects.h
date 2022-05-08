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
	};
	USING_SHARED_PTR(TcpListener);

	class Connection : public IOObject
	{
	public:
		Connection(uint32 key, IOProtocolPtr protocol) :
			IOObject(IO_OBJECT_TYPE_CONNECTION, key, protocol)
		{

		}
	};
	USING_SHARED_PTR(Connection);

	class Connector : public IOObject
	{
	public:
		Connector(uint32 key, IOProtocolPtr protocol) :
			IOObject(IO_OBJECT_TYPE_CONNECTOR, key, protocol),_state(EDisconnected)
		{

		}
	protected:
		EConnectState _state;
	};
	USING_SHARED_PTR(Connector);
}