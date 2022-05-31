#include "Protocol.h"
#include "IOPacket.h"
#include "IONotify.h"

namespace engine
{
	Protocol::Protocol(network::EPROTOCOL type):network::IOProtocol(type)
	{
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

	void Protocol::setQueue(MsgQueue<Packet*>* queue)
	{
		_msgqueue = queue;
	}

	void Protocol::dispatchPacket(Packet* packet)
	{
		_msgqueue->pushBack(packet);
	}

	void Protocol::onListen(bool success)
	{
		IONotify* packet = new IONotify(PacketListen, SHARED_THIS(Protocol), nullptr, success);
		dispatchPacket(packet);
	}

	void Protocol::onUnlisten()
	{
		IONotify* packet = new IONotify(PacketUnlisten, SHARED_THIS(Protocol));
		dispatchPacket(packet);
	}

	void Protocol::onAccept(const network::IOProtocolPtr &fromProtocol)
	{
		IONotify* packet = new IONotify(PacketAccept, SHARED_THIS(Protocol), std::dynamic_pointer_cast<Protocol>(fromProtocol), true);
		dispatchPacket(packet);
	}

	void Protocol::onClose()
	{
		IONotify* packet = new IONotify(PacketClose, SHARED_THIS(Protocol));
		dispatchPacket(packet);
	}

	void Protocol::onConnect(bool success)
	{
		IONotify* packet = new IONotify(PacketConnect, SHARED_THIS(Protocol), nullptr, success);
		dispatchPacket(packet);
	}

	void Protocol::onDisConnect()
	{
		IONotify* packet = new IONotify(PacketDisconnect, SHARED_THIS(Protocol));
		dispatchPacket(packet);
	}
}


