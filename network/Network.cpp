#include "core/TimerSet.h"
#include "Network.h"
#include "Common.h"
#include "Address.h"
#include "TCP_IOObjects.h"

#include "IOObject.h"
#include "IOObjects.h"
#include "IOProtocol.h"
#include "Poller.h"

#ifdef __linux
#include <signal.h>
#include "EPoller.h"
#endif // __linux

namespace net
{
	CNetwork::CNetwork() :_isRuning(false), _isStop(false),_poller(new CEPoller()),_shceduler(new timerset::TimerSet()), _lastclock(0)
	{
		_objects.resize(MAX_OBJECT_SIZE);
	}

	CNetwork::~CNetwork()
	{
		delete _poller;
		delete _shceduler;
	}

	void CNetwork::start()
	{
		_isRuning = true;
		_isStop = false;
		_thr = std::thread([this]() {
			run();
		});
	}

	void CNetwork::stop()
	{
		core_log_trace("net stop begine..");
		_isStop = true;
		_thr.join();
		_isRuning = false;
		core_log_trace("net stop end..");
	}

	bool CNetwork::isRuning()
	{
		return _isRuning;
	}

	uint32 CNetwork::listen(uint16 port, IOProtocolPtr protocol)
	{
		assert(!protocol->getKey());
		auto key = popKey();
		if (key == 0)
			return 0;
		protocol->setKey(key);
		protocol->setNetwork(this);
		protocol->setType(IO_OBJECT_LISTENER);
		pushEvent(static_cast<IOEvent*>(new IOListen(port, protocol)));
		return key;
	}

	uint32 CNetwork::connect(const std::string& ip, uint16 port, IOProtocolPtr protocol)
	{
		assert(!protocol->getKey());
		auto key = popKey();
		if (key == 0)
			return 0;
		protocol->setKey(key);
		protocol->setNetwork(this);
		protocol->setType(IO_OBJECT_CONNECTOR);
		pushEvent(static_cast<IOEvent*>(new IOEConnect(ip, port, protocol)));
		return key;
	}

	void CNetwork::close(uint32 key)
	{
		pushEvent(static_cast<IOEvent*>(new IOClose(key)));
	}

	void CNetwork::clear(int32 group)
	{
		pushEvent(static_cast<IOEvent*>(new IOEventClear(group)));
	}

	uint32 CNetwork::popKey()
	{
		return _keyPool.pop();
	}

	void CNetwork::pushKey(uint32 key)
	{
		_keyPool.push(key);
	}

	void CNetwork::addObject(const IOObjectPtr& object)
	{
		auto key = object->getKey();
		auto index = _keyPool.index(key);
		assert(_objects[index] == nullptr);
		_objects[index] = object;
		core_log_info("add object", object->getKey());
	}

	void CNetwork::removeObject(uint32 key)
	{
		auto index = _keyPool.index(key);
		auto& object = _objects[index];
		assert(object);
		assert(object->getKey() == key);
		object = nullptr;
		core_log_info("remove object", key);
	}

	IOObjectPtr CNetwork::getObject(uint32 key) const
	{
		auto index = _keyPool.index(key);
		auto object = _objects[index];
		if (object && object->getKey() == key)
		{
			return object;
		}
		return nullptr;
	}

	bool CNetwork::isEmpty() const
	{
		for (int32 i = 0; i < MAX_OBJECT_SIZE; i++)
			if (_objects[i])
				return false;
		return true;
	}

	std::list<IOObjectPtr> CNetwork::getGroupObject(int32 goroup) const
	{
		std::list<IOObjectPtr> objs;
		for (int32 i = 0; i< MAX_OBJECT_SIZE; ++i)
		{
			auto& obj = _objects[i];
			if (obj && (!goroup || obj->getGroup() == goroup || obj->getProtocol()->getGroup() == goroup))
			{
				objs.push_back(obj);
			}
		}
		return objs;
	}

	void CNetwork::run()
	{
		core_log_trace("start net:", std::this_thread::get_id());
		init();

		loop();

		onQuit();
	}

	void CNetwork::init()
	{
#ifdef __linux
		signal(SIGPIPE, SIG_IGN);
#endif // __linux
	}

	void CNetwork::loop()
	{
		while (!_isStop)
		{
			_poller->poll(_objects, 4);

			std::list<IOEvent*> events;
			_eventQueue.pop(events);

			auto final = CFinalize([&events]() -> void {
				for (auto iter : events)
				{
					delete iter;
				}
			});

			for (auto iter = events.begin(); iter != events.end(); ++iter)
			{
				dispatchProcess(*iter);
			}

			auto now = TimeHelp::clock().count();
			if (now - _lastclock >= 1000)
			{
				onTimer1000ms();
				_lastclock = now;
			}
			_shceduler->update(now);
		}
	}

	void CNetwork::onQuit()
	{
		if(!_eventQueue.empty())
		{
			std::list<IOEvent*> events;
			_eventQueue.pop(events);

			auto final = CFinalize([&events]() -> void {
				for (auto iter : events)
				{
					delete iter;
				}
			});

			for (auto iter = events.begin(); iter != events.end(); ++iter)
			{
				dispatchProcess(*iter);
			}			
		}

		if (!_eventQueue.empty())
			core_log_error("net quit existed event ");

		if (!isEmpty())
		{
			cleartImpl(0);

			while (!isEmpty())
			{
				_poller->poll(_objects, 4);

				auto now = TimeHelp::clock().count();
				_shceduler->update(now);
			}
		}
	}

	void CNetwork::onTimer1000ms()
	{

	}

	void CNetwork::pushEvent(IOEvent* event)
	{
		bool notify;
		_eventQueue.push(event, notify);
	}

	void CNetwork::dispatchProcess(IOEvent* event)
	{
		switch (event->getType())
		{
		case IO_EVENT_LISTEN:
		{
			processListen(dynamic_cast<IOListen*>(event));
			break;
		}
		case IO_EVENT_CONNECT:
		{
			processConnect(dynamic_cast<IOEConnect*>(event));
			break;
		}
		case IO_EVENT_DATA:
		{
			processDataEvent(event);
			break;
		}
		case IO_EVENT_CLOSE:
		{
			processClose(dynamic_cast<IOClose*>(event));
			break;
		}
		case IO_EVENT_CLEAR:
		{
			processClear(dynamic_cast<IOEventClear*>(event));
			break;
		}
		default:
			core_log_error("unknow process", event->getType());
			break;
		}
	}

	void CNetwork::processDataEvent(IOEvent* event)
	{
		auto key = event->getKey();
		auto object = getObject(key);
		if (!object)
		{
			core_log_error("process data object null", key);
			return;
		}
		auto protocol = object->getProtocol();
		switch (protocol->getProtocolType())
		{
		case EPROTO_TCP:
			tcpSend(object, event);
			break;
		case EPROTO_KCP:
			break;
		default:
			assert(false);
			break;
		}
	}

	void CNetwork::processListen(IOListen* event)
	{
		auto protocol = event->getProtocol();
		auto protoType = protocol->getProtocolType();
		switch (protoType)
		{
		case EPROTO_TCP:
		{
			tcpListen(event->getPort(), protocol);
			break;
		}
		case EPROTO_KCP:
		{
			break;
		}
		default:
			assert(false);
			break;
		}
	}

	void CNetwork::processConnect(IOEConnect* event)
	{
		auto protocol = event->getProtocol();
		auto protoType = protocol->getProtocolType();
		switch (protoType)
		{
		case EPROTO_TCP:
		{
			tcpConnect(event->getIp(), event->getPort(), protocol);
			break;
		}
		case EPROTO_KCP:
		{
			break;
		}
		default:
			assert(false);
			break;
		}
	}

	void CNetwork::processClose(IOClose* event)
	{
		closeImpl(event->getKey());
	}

	void CNetwork::processClear(IOEventClear* event)
	{
		cleartImpl(event->getGroup());
	}

	void CNetwork::closeImpl(uint32 key)
	{
		auto object = getObject(key);
		if (!object)
		{
			core_log_error("process data object null", key);
			return;
		}
		auto protocol = object->getProtocol();
		switch (protocol->getProtocolType())
		{
		case EPROTO_TCP:
			tcpClose(key, false);
			break;
		case EPROTO_KCP:
			break;
		default:
			assert(false);
			break;
		}
	}

	void CNetwork::cleartImpl(int32 group)
	{
		auto objs = getGroupObject(group);
		for (auto& obj : objs)
			closeImpl(obj->getKey());
	}

	void CNetwork::defaultErrorHandle(const IOObjectPtr& object)
	{
		auto err = object->getEndPoint()->getSocketError();
		core_log_error("default error", object->getKey(), object->getType(), err, strerror(err));
	}
}
