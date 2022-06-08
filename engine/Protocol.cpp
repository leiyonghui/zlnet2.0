#include "Protocol.h"
#include "IOPacket.h"
#include "IONotify.h"

namespace engine
{

	void Protocol::onAwake(EPROTOCOL pType)
	{
		net::IOProtocol::onAwake(pType);
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

	void Protocol::dispatchPacket(PacketPtr packet)
	{
		_msgqueue->pushBack(packet);
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
}


