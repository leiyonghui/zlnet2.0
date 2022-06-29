#include "IOObject.h"

namespace net
{
	IOObject::IOObject(EIOOBJECT_TYPE type): _type(type),_key(0),_group(0),_events(0)
	{

	}

	void IOObject::onAwake(const IOProtocolPtr& protocl, CEndPointUnPtr endPoint)
	{
		_protocol = protocl;
		_key = protocl->getKey();
		_events = 0;
		_endpoint = std::move(endPoint);
		_endpoint->setNonblock();
	}

	void IOObject::onRecycle()
	{
		_protocol = nullptr;
		_endpoint = nullptr;
		TimerImpl<IOObject>::reset();
	}
}