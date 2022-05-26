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
}