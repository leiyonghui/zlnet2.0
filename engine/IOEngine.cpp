#include "network/Network.h"
#include "IOEngine.h"

namespace engine
{
	IOEngine::IOEngine():_network(new network::CNetwork())
	{
		bindMsgdispatcher(std::bind(&IOEngine::dispatchIOPacket, this, _1));
	}

	IOEngine::~IOEngine()
	{
		delete _network;
	}

	void IOEngine::dispatchIOPacket(Packet* packet)
	{
		switch (packet->getType())
		{
		case PacketListen:
			onIOListen(packet);
			break;
		case PacketUnlisten:
			onIOUnListen(packet);
			break;
		case PacketAccept:
			onIOAccept(packet);
			break;
		case PacketConnect:
			onIOConnect(packet);
			break;
		case PacketClose:
			onIOClose(packet);
			break;
		case PacketMsg:
			onIOPacket(packet);
			break;
		default:
			assert(false);
			break;
		}
	}

	void IOEngine::onListen(uint32 uid)
	{

	}

	void IOEngine::onUnListen(uint32 uid)
	{
	}

	void IOEngine::onAccept(uint32 uid)
	{
	}

	void IOEngine::onClose(uint32 uid)
	{
	}

	void IOEngine::onConnect(uint32 uid, bool success)
	{
	}

	void IOEngine::onDisconnect(uint32 uid)
	{
	}

	void IOEngine::onIOListen(Packet * packet)
	{
	}

	void IOEngine::onIOUnListen(Packet * packet)
	{

	}

	void IOEngine::onIOAccept(Packet * packet)
	{

	}

	void IOEngine::onIOClose(Packet * packet)
	{

	}

	void IOEngine::onIOConnect(Packet * packet)
	{

	}

	void IOEngine::onIODisconnect(Packet * packet)
	{

	}

	void IOEngine::onIOPacket(Packet * packet)
	{
	}
}