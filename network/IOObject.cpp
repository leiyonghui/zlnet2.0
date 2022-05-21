#include "IOObject.h"
#include "IOObject.h"

namespace network
{
	IOObject::IOObject(IO_OBJECT_TYPE type):_type(type),_key(0),_events(0)
	{

	}

	void IOObject::onAwake(const IOProtocolPtr& protocl, CEndPointUnPtr endPoint)
	{
		_protocol = protocl;
		_key = protocl->getKey();
		_events = 0;
		_endpoint = std::move(endPoint);
	}

	void IOObject::onRecycle()
	{
		_protocol = nullptr;
	}
}