#include "Network.h"
#include "Common.h"
#include "Address.h"
#include "TcpIOObjects.h"

#ifdef __linux
#include <signal.h>
#include "EPoller.h"
#endif // __linux

namespace network
{
	CNetwork::CNetwork() :_isStop(false),_poller(new CEPoller())
	{
		_objects.resize(MAX_OBJECT_SIZE);
	}

	CNetwork::~CNetwork()
	{
		delete _poller;
	}

	void CNetwork::addObject(const IOObjectPtr& object)
	{
		auto key = object->getKey();
		assert(_objects[key] == nullptr);
		_objects[key] = object;
	}

	void CNetwork::removeObject(uint32 key)
	{
		auto index = _keyPool.index(key);
		auto& object = _objects[index];
		assert(object);
		assert(object->getKey() == key);
		object = nullptr;
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

	void CNetwork::start()
	{
		std::thread ([this]() {
			loop();
		}).detach();
	}

	uint32 CNetwork::listen(uint16 port, IOProtocolPtr protocol)
	{
		auto key = popKey();
		if (key == 0)
			return 0;
		protocol->setKey(key);
		pushEvent(std::static_pointer_cast<IOEvent>(IOListenPtr(new IOListen(port, protocol))));
		return key;
	}

	uint32 CNetwork::connect(const std::string& ip, uint16 port, IOProtocolPtr protocol)
	{
		return uint32();
	}

	uint32 CNetwork::close(uint32 key)
	{
		return uint32();
	}

	uint32 CNetwork::popKey()
	{
		return _keyPool.pop();
	}

	void CNetwork::pushKey(uint32 key)
	{
		_keyPool.push(key);
	}

	void CNetwork::init()
	{
#ifdef __linux
		signal(SIGPIPE, SIG_IGN);
#endif // __linux
	}

	void CNetwork::loop()
	{
		while (_isStop)
		{
			_poller->poll(1);

			std::list<IOEvent*> events;
			_eventQueue.pop(events);

			for (std::list<IOEvent*>::iterator iter = events.begin(); iter != events.end(); ++iter)
			{
				dispatchProcess(*iter);
				delete* iter;
			}
		}
	}

	void CNetwork::dispatchProcess(IOEvent* event)
	{
		switch (event->getType())
		{
		case IO_EVENT_LISTEN:
		{
			processListen(event);
			break;
		}
		case IO_EVENT_DATA:
		{
			processData(event);
			break;
		}
		case IO_EVENT_CONNECT:
		{
			processConnect(event);
			break;
		}
		case IO_EVENT_CLOSE:
		{
			processClose(event);
			break;
		}
		default:
			core_log_error("unknow process", event->getType());
			break;
		}
	}

	void CNetwork::pushEvent(IOEvent* event)
	{
		bool notify;
		_eventQueue.push(event, notify);
	}

	void CNetwork::processListen(IOEvent* event)
	{
		IOListen* ev = dynamic_cast<IOListen*>(event);
		auto protocol = ev->getProtocol();
		auto protoType = protocol->getProtocolType();
		if (protoType == EPROTO_TCP)
		{
			tcpListen(ev->getPort(), protocol);
		}
		else if (protoType == EPROTO_UDP)
		{
			
		}
		else if (protoType == EPROTO_KCP)
		{

		}
	}

	void CNetwork::processData(IOEvent* event)
	{

	}

	void CNetwork::processConnect(IOEvent* event)
	{

	}

	void CNetwork::processClose(IOEvent* event)
	{

	}

	void CNetwork::defaultErrorHandle(const IOObjectPtr& object)
	{
		core_log_error("default error", object->getKey(), object->getType());
	}
}
