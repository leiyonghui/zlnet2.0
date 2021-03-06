#pragma once
#include "IOObjects.h"

namespace net
{
	class TcpListener : public IOObject
	{
	public:
		TcpListener();
		void onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint);
		void onRecycle();

		bool listen();
		CEndPointUnPtr accept();
	};

	class TcpSession : public Connection
	{
	public:
		TcpSession():Connection(IO_OBJECT_SESSION){}
		void onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint);
		void onRecycle();
	};

	class TcpConnector : public Connection
	{
	public:
		TcpConnector():Connection(IO_OBJECT_CONNECTOR){}
		void onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint);
		void onRecycle();

		void setEndPoint(CEndPointUnPtr&& endPoint);
	};
}