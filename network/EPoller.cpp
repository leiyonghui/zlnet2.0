#ifdef __linux
#include <sys/epoll.h>
#include <unistd.h>
#include "EPoller.h"

namespace network
{
	void printfEvent(int32 fd, int32 ev)
	{
		std::ostringstream oss;
		oss << "fd:" << fd << " ";
		if (ev & EPOLLIN)
			oss << "IN ";
		if (ev & EPOLLPRI)
			oss << "PRI ";
		if (ev & EPOLLOUT)
			oss << "OUT ";
		if (ev & EPOLLHUP)
			oss << "HUP ";
		if (ev & EPOLLRDHUP)
			oss << "RDHUP ";
		if (ev & EPOLLERR)
			oss << "ERR ";
		//if (ev & EPOLLNVAL)
		//	oss << "NVAL ";
		std::string str = oss.str();
		printf("%s\n", str.c_str());
	}

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

	void CEPoller::poll(int32 millisecond)
	{
		int32 cnt = ::epoll_wait(_epfd, _events, MAX_OBJECT_SIZE, millisecond);
		for (int32 i = 0; i < cnt; i++)
		{
			const epoll_event& event = _events[i];
			IOObjectPtr object = getObject(event.data.u32);
			const uint32& ev = event.events;
			int32 events = object->getEvents();
			printfEvent(object->getSocket(), ev);
			if (ev & EPOLLERR)
			{
				auto& callback = object->getErrorCallback();
				assert(callback);
				callback(object);
			}
			else
			{
				if (ev & EPOLLIN & events)
				{
					auto& callback = object->getReadCallback();
					if (callback)
					{
						callback(object);
					}
				}
				if (ev & EPOLLOUT & events)
				{
					auto& callback = object->getWriteCallback();
					if (callback)
					{
						callback(object);
					}
				}
			}
		}
	}

	void CEPoller::updateObject(const IOObjectPtr& object, int32 events)
	{
		//if (events == 0)
		//{
		//	removeObject(object);
		//	return;
		//}
		epoll_event event;
		event.events = events;
		event.data.u32 = object->getKey();
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
			core_log_error("unexpect", object->getKey());
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

