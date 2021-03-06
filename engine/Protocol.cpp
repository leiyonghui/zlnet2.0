#include "Protocol.h"
#include "IOPacket.h"
#include "IONotify.h"

namespace engine
{
	const int32 MAX_HEART_COUNT = 60;

	Protocol::Protocol():net::IOProtocol(),_available(false),_heartbeat(0)
	{
	}

	void Protocol::onAwake(EPROTOCOL pType)
	{
		net::IOProtocol::onAwake(pType);
		_available = false;
		_heartbeat = 0;
	}

	void Protocol::onRecycle()
	{
		net::IOProtocol::onRecycle();
	}

	bool Protocol::isAvailable() const
	{
		return _available;
	}

	void Protocol::setAvailable()
	{
		_available = true;
	}

	void Protocol::unsetAvailable()
	{
		_available = false;
	}

	void Protocol::setQueue(MsgQueue<PacketPtr>* queue)
	{
		_msgqueue = queue;
	}

	CallbackHandlerExistList& Protocol::getCallbackList()
	{
		return _callbacklist;
	}

	int32 Protocol::getHeartCount() const
	{
		return _heartbeat;
	}

	void Protocol::setHeartCount(int32 count)
	{
		_heartbeat = count;
	}

	void Protocol::dispatchPacket(PacketPtr packet)
	{
		_msgqueue->pushBack(packet);
	}

	void Protocol::handlerHeart()
	{
		setHeartCount(0);
	}

	void Protocol::onListen(bool success)
	{
		IONotifyPtr packet = std::make_shared<IONotify>(PacketListen, SHARED_THIS(Protocol), nullptr, success);
		dispatchPacket(packet);
	}

	void Protocol::onUnlisten()
	{
		IONotifyPtr packet = std::make_shared<IONotify>(PacketUnlisten, SHARED_THIS(Protocol));
		dispatchPacket(packet);
	}

	void Protocol::onAccept(const net::IOProtocolPtr &fromProtocol)
	{
		IONotifyPtr packet = std::make_shared<IONotify>(PacketAccept, SHARED_THIS(Protocol), std::dynamic_pointer_cast<Protocol>(fromProtocol), true);
		dispatchPacket(packet);
	}

	void Protocol::onClose()
	{
		IONotifyPtr packet = std::make_shared<IONotify>(PacketClose, SHARED_THIS(Protocol));
		dispatchPacket(packet);
	}

	void Protocol::onConnect(bool success)
	{
		IONotifyPtr packet = std::make_shared<IONotify>(PacketConnect, SHARED_THIS(Protocol), nullptr, success);
		dispatchPacket(packet);
	}

	void Protocol::onDisConnect()
	{
		IONotifyPtr packet = std::make_shared<IONotify>(PacketDisconnect, SHARED_THIS(Protocol));
		dispatchPacket(packet);
	}

	bool Protocol::onTimer()
	{
		/*if (getType() == net::IO_OBJECT_SESSION)
		{
			auto count = getHeartCount() + 1;
			if (count > MAX_HEART_COUNT)
			{
				core_log_trace("protocol heart close", _key, count);
				return false;
			}
			else
			{
				setHeartCount(count);
			}
		}*/
		return true;
	}
}


