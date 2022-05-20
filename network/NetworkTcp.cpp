#include "network/Network.h"

namespace network
{

	void CNetwork::handleTcpAccept(const IOObjectPtr& object)
	{
		auto listener = std::dynamic_pointer_cast<TcpListener>(object);
		assert(listener);
		auto protocol = listener->getProtocol();
		auto endPoint = listener->accept();
		if (!endPoint) 
		{
			return;
		}
		auto key = popKey();
		if (!key) 
		{
			CORE_LOG_ERROR("not key accept");
			return;
		}
		auto conPrototcol = protocol->create();
		conPrototcol->setKey(key);
		auto conn = CObjectPool<TcpConnection>::Instance()->create(conPrototcol, std::move(endPoint));
		conn->setErrorCallback();
		conn->setReadCallback();
		addObject(conn);
		protocol->onAccept(conPrototcol);
	}
}