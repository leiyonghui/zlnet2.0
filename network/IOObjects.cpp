#include "IOObjects.h"
#include "IOObjects.h"
#include "IOObjects.h"
#pragma once
#include "IOObjects.h"

namespace network
{
	void TcpListener::onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint)
	{
		IOObject::onAwake(IO_OBJECT_TYPE_LISTENER, protocol, std::move(endPoint));
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
		IOObject::onAwake(IO_OBJECT_TYPE_CONNECTION, protocol, std::move(endPoint));
	}

	void TcpConnection::onRecycle()
	{
		IOObject::onRecycle();
	}
}