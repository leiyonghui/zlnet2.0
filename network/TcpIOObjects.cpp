#include "TcpIOObjects.h"

namespace network
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

	void TcpConnection::onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint)
	{
		Connection::onAwake(protocol, std::move(endPoint));
	}

	void TcpConnection::onRecycle()
	{
		Connection::onRecycle();
	}

	CEndPoint* TcpConnection::getEndPoint()
	{
		return _endpoint.get();
	}

	void TcpConnector::onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint)
	{
		Connection::onAwake(protocol, std::move(endPoint));
	}

	void TcpConnector::onRecycle()
	{
		Connection::onRecycle();
	}
}