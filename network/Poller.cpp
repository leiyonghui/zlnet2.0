#include "Poller.h"

namespace net
{
	void CPoller::registerReadHandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() | READ_EVENT /*EPOLLIN*/);
	}

	void CPoller::registerWriteHandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() | WRITE_EVENT /*EPOLLOUT*/);
	}

	void CPoller::deregisterReadHandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() ^ READ_EVENT /*EPOLLIN*/);
	}

	void CPoller::deregisterWriteHandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() ^ WRITE_EVENT /*EPOLLOUT*/);
	}

	void CPoller::deregisterObject(const IOObjectPtr& object)
	{
		removeObject(object);
	}
}