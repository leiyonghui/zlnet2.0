#include "Protocol.h"
#include "IONotify.h"
#include "IOPacket.h"

namespace engine
{
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

	void Protocol::onListen()
	{

	}

	void Protocol::onUnlisten()
	{

	}

	void Protocol::onAccept(network::IOProtocolPtr &fromProtocol)
	{
		IONotify* packet = new IONotify(PacketAccept, SHARED_THIS(Protocol), std::dynamic_pointer_cast<Protocol>(fromProtocol), true);
		dispatchPacket(packet);
	}

	void Protocol::onClose()
	{

	}

	void Protocol::onConnect(bool dis)
	{

	}

	void Protocol::onDisConnect()
	{

	}

	void Protocol::dispatchPacket(Packet* packet)
	{
		
	}
}


