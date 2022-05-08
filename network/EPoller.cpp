#ifdef __linux
#include "EPoller.h"
#include <sys/epoll.h>
#include <unistd.h>

namespace network
{
	CEPoller::CEPoller():_epfd(epoll_create(MAX_OBJECT_SIZE)), _events(new epoll_event[MAX_OBJECT_SIZE])
	{
	}
	CEPoller::~CEPoller()
	{
		delete[] _events;
	}

	void CEPoller::registerReadHandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() & EPOLLIN);
	}

	void CEPoller::registerWriteHandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() & EPOLLOUT);
	}

	void CEPoller::deregisterReadhandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() ^ EPOLLIN);
	}

	void CEPoller::deregisterWritehandler(const IOObjectPtr& object)
	{
		updateObject(object, object->getEvents() ^ EPOLLOUT);
	}

	void CEPoller::deregisterObject(const IOObjectPtr& object)
	{
		removeObject(object);
	}

	void CEPoller::poll()
	{

	}

	void CEPoller::updateObject(const IOObjectPtr& object, int32 events)
	{
		if (events == 0)
		{
			removeObject(object);
			return;
		}
		epoll_event event;
		event.events = events;
		event.data.u32 = object->key();
		auto socket = object->getSocket();
		if (hasObject(object))
		{
			if (::epoll_ctl(_epfd, EPOLL_CTL_MOD, socket, &event))
			{
				core_log_error("epoll mod error", errno);
				return;
			}
		}
		else
		{
			if (::epoll_ctl(_epfd, EPOLL_CTL_ADD, socket, &event) < 0)
			{
				core_log_error("epoll add error", errno);
				return;
			}
		}
		object->updateEvents(events);
	}

	void CEPoller::removeObject(const IOObjectPtr& object)
	{
		if (!hasObject(object))
		{
			core_log_error("unexpect", object->key());
			return;
		}
		auto socket = object->getSocket();
		if (::epoll_ctl(_epfd, EPOLL_CTL_DEL, socket, NULL))
		{
			core_log_error("epoll del", errno);
			return;
		}
	}
}
#endif // __liunx

