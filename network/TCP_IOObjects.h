#pragma once
#include "IOObjects.h"

namespace network
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

	class TcpConnection : public Connection
	{
	public:
		TcpConnection():Connection(IO_OBJECT_CONNECTION){}
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