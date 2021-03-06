#ifdef __linux
#include <sys/epoll.h>
#include <unistd.h>
#include "EPoller.h"

namespace net
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

	void CEPoller::poll(const std::vector<IOObjectPtr>& objects, int32 millisecond)
	{
		int32 cnt = ::epoll_wait(_epfd, _events, MAX_OBJECT_SIZE, millisecond);
		for (int32 i = 0; i < cnt; i++)
		{
			const epoll_event& event = _events[i];
			auto key = event.data.u32;
			auto index = ObjecKeyPool::index(key);
			auto object = objects[index];
			if (!object || object->getKey() != key)
			{
				core_log_warning("object null", key, object ? object->getKey() : 0);
				continue;
			}
			const uint32& ev = event.events;
			//printfEvent(object->getSocket(), ev);
			if (ev & EPOLLERR)
			{
				auto& callback = object->getErrorCallback();
				assert(callback);
				callback(object);
			}
			else
			{
				if ((ev & (EPOLLIN | EPOLLHUP)) && object->hasReading())
				{
					auto& callback = object->getReadCallback();
					if (callback)
						callback(object);
				}
				if ((ev & (EPOLLOUT | EPOLLHUP)) && object->hasWriting())
				{
					auto& callback = object->getWriteCallback();
					if (callback)
						callback(object);
				}
			}
		}
	}

	void CEPoller::updateObject(const IOObjectPtr& object, int32 events)
	{
		if (events == 0)
		{
			removeObject(object);
			return;
		}
		epoll_event event;
		event.events = 0;
		if (events & READ_EVENT)
			event.events |= EPOLLIN;
		if (events & WRITE_EVENT)
			event.events |= EPOLLOUT;
		event.data.u32 = object->getKey();
		auto socket = object->getSocket();
		if (object->getEvents())
		{
			object->updateEvents(events);
			if (::epoll_ctl(_epfd, EPOLL_CTL_MOD, socket, &event))
			{
				core_log_error("epoll mod error", errno);
				return;
			}
		}
		else
		{
			object->updateEvents(events);
			if (::epoll_ctl(_epfd, EPOLL_CTL_ADD, socket, &event) < 0)
			{
				core_log_error("epoll add error", errno);
				return;
			}
		}
	}

	void CEPoller::removeObject(const IOObjectPtr& object)
	{
		auto socket = object->getSocket();
		object->updateEvents(0);
		if (::epoll_ctl(_epfd, EPOLL_CTL_DEL, socket, NULL))
		{
			core_log_error("epoll del", errno);
			return;
		}
	}
}
#endif // __liunx

