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
		void onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint);
		void onRecycle();

		CEndPoint* getEndPoint();
	};

	class TcpConnector : public Connection
	{
	public:
		void onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint);
		void onRecycle();
	protected:
		EConnectState _state;
	};
}