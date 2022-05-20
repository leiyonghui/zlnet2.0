#include "Network.h"
#include "Common.h"
#include "Address.h"
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
			_poller->poll();

			std::list<IOEventPtr> events;
			_eventQueue.pop(events);

			for (std::list<IOEventPtr>::iterator iter = events.begin(); iter != events.end(); ++iter)
			{
				dispatchProcess(*iter);
			}
		}
	}

	void CNetwork::dispatchProcess(const IOEventPtr& event)
	{
		switch (event->getType())
		{
		case IO_EVENT_LISTEN:
		{
			processListen(event);
			break;
		}
		default:
			break;
		}
	}

	void CNetwork::pushEvent(const IOEventPtr& event)
	{
		bool notify;
		_eventQueue.push(event, notify);
	}

	void CNetwork::processListen(const IOEventPtr& event)
	{
		IOObjectPtr object = nullptr;
		IOListenPtr ev = std::dynamic_pointer_cast<IOListen>(event);
		auto protocol = ev->getProtocol();
		auto protoType = protocol->getProtocolType();
		if (protoType == EPROTO_TCP)
		{
			auto socket = common::CreateSocket(EPROTO_TCP);
			auto address = CAddress("", ev->_port);
			auto endPoint = CObjectPool<CEndPoint>::Instance()->createUnique(socket, address);
			auto listener = CObjectPool<TcpListener>::Instance()->create(protocol, std::move(endPoint));
			listener->setReadCallback(std::bind(&CNetwork::handleTcpAccept, this, _1));
			if (!listener->listen()) 
			{
				protocol->onUnlisten();
				return;
			}
		}
		else if (protoType == EPROTO_UDP)
		{
			
		}
		addObject(object);
		_poller->deregisterReadhandler(object);
	}

}
