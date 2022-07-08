#include "IOObject.h"

namespace net
{
	IOObject::IOObject(EIOOBJECT_TYPE type): _type(type),_key(0),_group(0),_events(0)
	{

	}

    IOObject::IOObject(EIOOBJECT_TYPE type, uint32 key, CEndPointUnPtr&& endPoint):_type(type), _key(key), _group(0), _events(0), _endpoint(std::move(endPoint))
    {

    }

	void IOObject::onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint)
	{
		_protocol = protocol;
		_key = protocol->getKey();
		_events = 0;
		_group = 0;
		_endpoint = std::move(endPoint);
		_endpoint->setNonblock();
	}

	void IOObject::onRecycle()
	{
		_protocol = nullptr;
		_endpoint = nullptr;
	}
}