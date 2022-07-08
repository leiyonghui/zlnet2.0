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
	const int32 POLL_TIMEOUT = 10 * 1000;

	CNetwork::CNetwork() :_isRuning(false), _isStop(false),_poller(new CEPoller()),_shceduler(new timerset::TimerSet()), _tick(0), _lastclock(0), _secondTick(0)
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
		if (!init())
		{
			core_log_error("net init");
			return;
		}
		core_log_trace("start net:", std::this_thread::get_id());

		loop();

		onQuit();
	}

	bool CNetwork::init()
	{
#ifdef __linux
		signal(SIGPIPE, SIG_IGN);
#endif // __linux

		auto key = popKey();
		if (!key)
		{
			core_log_error("key null");
			return false;
		}
		auto socket = common::CreateSocket(EPROTO_TCP);
		if (socket == INVALID_SOCKET)
		{
			core_log_error("listen create socket");
			return false;
		}
		auto endPoint = CObjectPool<CEndPoint>::Instance()->createUnique(socket);
		_wakeupObject = std::make_shared<IOObject>(IO_OBJECT_WAKEUP, key, std::move(endPoint));
		_wakeupObject->setReadCallback(std::bind(&CNetwork::weakupReadHandler, this, _1));
		_poller->registerReadHandler(_wakeupObject);
		addObject(_wakeupObject);
	}

	void CNetwork::loop()
	{
		while (!_isStop)
		{
			++_tick;
			_poller->poll(_objects, POLL_TIMEOUT);

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
			_clear(0);

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
		++_secondTick;

#ifdef _MONITOR
		if (_secondTick % 30 == 1)
		{
			core_log_info("----------Buffer Size----------");
			for (auto& obj : _objects)
			{
				if (!obj || obj->getType() == IO_OBJECT_LISTENER)
					continue;

				auto con = std::dynamic_pointer_cast<Connection>(obj);
				if (con->getInputBuffer()->size())
					core_log_trace("con:", con->getKey(), "input:", con->getInputBuffer()->size(), con->getInputBuffer()->capacity());
				if (con->getOutBuffer()->size())
					core_log_trace("con:", con->getKey(), "output:", con->getOutBuffer()->size(), con->getOutBuffer()->capacity());
			}
		}
#endif

	}

	void CNetwork::pushEvent(IOEvent* event)
	{
		bool notify;
		_eventQueue.push(event, notify);
		if (notify) 
			weakup();
	}

    void CNetwork::weakup()
    {
		int32 n = 1;
		auto cnt = _wakeupObject->getEndPoint()->write((char*)&n, sizeof(n));
		if (cnt != sizeof(n)) 
		{
			core_log_error("weakup error", cnt);
		}
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
		_close(event->getKey());
	}

	void CNetwork::processClear(IOEventClear* event)
	{
		_clear(event->getGroup());
	}

	void CNetwork::_close(uint32 key)
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

	void CNetwork::_clear(int32 group)
	{
		auto objs = getGroupObject(group);
		for (auto& obj : objs)
			_close(obj->getKey());
	}

	void CNetwork::weakupReadHandler(const IOObjectPtr& object)
	{
		auto endPoint = object->getEndPoint();
		int32 n = 0;
		auto cnt = endPoint->read((char*)&n, sizeof(n));
		if (cnt != sizeof(n)) 
		{
			core_log_error("weakup unexpect", cnt);
		}
	}

	void CNetwork::defaultErrorHandle(const IOObjectPtr& object)
	{
		auto err = object->getEndPoint()->getSocketError();
		core_log_error("default error", object->getKey(), object->getType(), err, strerror(err));
	}

	void CNetwork::onNewConnection(const ConnectionPtr& con)
	{
		con->startTimer(1000ms, 1000ms, [this](ConnectionPtr con) {
			if (!con->getProtocol()->onTimer())
			{
				_close(con->getKey());
			}
		});

#ifdef _MONITOR
		/*con->startTimer(0ms, 30s, [](ConnectionPtr con) {
			core_log_trace("con:", con->getKey(), "input:", con->getInputBuffer()->size(), con->getInputBuffer()->capacity());
			core_log_trace("con:", con->getKey(), "output:", con->getOutBuffer()->size(), con->getOutBuffer()->capacity());
		});*/
#endif // _MONITOR

	}
}
