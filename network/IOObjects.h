#pragma once
#include "IOObject.h"

namespace network
{
	class TcpListener : public IOObject
	{
	public:
		TcpListener() = default;
		virtual ~TcpListener() = default;

		void onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint);
		void onRecycle();

		bool listen();
		CEndPointUnPtr accept();
	};

	class TcpConnection : public IOObject
	{
	public:
		TcpConnection(uint32 key, IOProtocolPtr protocol) :
			IOObject(IO_OBJECT_TYPE_CONNECTION, key, protocol)
		{

		}
		void onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint);
		void onRecycle();
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