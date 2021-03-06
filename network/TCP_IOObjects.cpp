#include "TCP_IOObjects.h"

namespace net
{
	TcpListener::TcpListener():IOObject(IO_OBJECT_LISTENER)
	{

	}

	void TcpListener::onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint)
	{
		IOObject::onAwake(protocol, std::move(endPoint));
	}

	void TcpListener::onRecycle()
	{
		IOObject::onRecycle();
	}

	bool TcpListener::listen()
	{
		return !_endpoint->listen();
	}

	CEndPointUnPtr TcpListener::accept()
	{
		return _endpoint->accept();
	}

	void TcpSession::onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint)
	{
		Connection::onAwake(protocol, std::move(endPoint));
	}

	void TcpSession::onRecycle()
	{
		Connection::onRecycle();
	}

	void TcpConnector::onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint)
	{
		Connection::onAwake(protocol, std::move(endPoint));
	}

	void TcpConnector::onRecycle()
	{
		Connection::onRecycle();
	}

	void TcpConnector::setEndPoint(CEndPointUnPtr&& endPoint)
	{
		_endpoint = std::move(endPoint);
	}
}