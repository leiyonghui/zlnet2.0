#include "IOObject.h"

namespace network
{
	void IOObject::onAwake(IO_OBJECT_TYPE type, const IOProtocolPtr& protocl, CEndPointUnPtr endPoint)
	{
		_type = type;
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