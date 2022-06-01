#include "Poller.h"

namespace network
{
	void CPoller::registerReadHandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() | READ_EVENT);
	}

	void CPoller::registerWriteHandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() | WRITE_EVENT);
	}

	void CPoller::deregisterReadHandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() ^ READ_EVENT);
	}

	void CPoller::deregisterWriteHandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() ^ WRITE_EVENT);
	}

	void CPoller::deregisterObject(const IOObjectPtr& object)
	{
		removeObject(object);
	}
}