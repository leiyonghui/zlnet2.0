#include "Network.h"

#ifdef __linux
#include <signal.h>
#endif // __linux

namespace network
{
	CNetwork::CNetwork() :_isStop(false)
	{
		_objects.resize(MAX_OBJECT_SIZE);
	}

	CNetwork::~CNetwork()
	{

	}

	bool CNetwork::addObject(const IOObjectPtr& object)
	{
		return false;
	}

	IOObjectPtr CNetwork::getObject(uint32 key) const
	{
		auto index = _keyPool.index(key);
		auto object = _objects[index];
		if (object && object->key() == key)
		{
			return object;
		}
		return nullptr;
	}
	uint32 CNetwork::popKey()
	{
		return _keyPool.pop();
	}

	void CNetwork::pushKey(uint32 key)
	{
		_keyPool.push(key);
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
		pushEvent(std::static_pointer_cast<IOEvent>(IOListenPtr(new IOListen(port))));
	}

	uint32 CNetwork::connect(const std::string& ip, uint16 port, IOProtocolPtr protocol)
	{
		return uint32();
	}

	uint32 CNetwork::close(uint32 key)
	{
		return uint32();
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
		IOListenPtr ev = std::dynamic_pointer_cast<IOListen>(event);
		auto protocol = ev->getProtocol();
		TcpListenerPtr object = std::make_shared<TcpListener>(ev->getKey(), protocol);
		if (!addObject(object))
		{
			core_log_error("listen add object error", ev->getKey());
			return;
		}
		object->setReadCallback(std::bind(&CNetwork::handPollTcpAccept, this, _1));
	}
	void CNetwork::handPollTcpAccept(const IOObjectPtr& object)
	{
	}
}
