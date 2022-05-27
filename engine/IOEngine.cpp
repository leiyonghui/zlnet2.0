#include "network/Network.h"
#include "IOEngine.h"
#include "IONotify.h"
#include "IOPacket.h"

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

	ProtocolPtr IOEngine::getProtocol(uint32 key)
	{
		auto iter = _protocols.find(key);
		if (iter == _protocols.end()) 
		{
			return nullptr;
		}
		return iter->second;
	}

	uint32 IOEngine::listen(uint16 port, const ProtocolPtr& protocol)
	{
		if (protocol->getKey())
		{
			core_log_error("listen protocol unexpected");
			return;
		}
		protocol->setNetwork(_network);
		auto key = _network->listen(port, protocol);
		if (key <= 0) 
		{
			core_log_error("listen unexpected");
			return 0;
		}
		if (!core::insert(_protocols, key, protocol)) 
		{
			core_log_error("listen unexpected");
			return 0;
		}
		return key;
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

	void IOEngine::onListen(uint32 uid, bool success)
	{
		
	}

	void IOEngine::onUnlisten(uint32 uid)
	{
	}

	void IOEngine::onAccept(uint32 uid, uint32 fromUid)
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
		IONotify* notify = dynamic_cast<IONotify*>(packet);
		auto uid = notify->getUid();
		auto protocol = getProtocol(uid);
		if (protocol != notify->getProtocol())
		{
			core_log_error("unexpect", uid);
			return;
		}
		if (notify->isSuccess())
		{
			protocol->setAvailable();
		}
		onListen(uid, notify->isSuccess());
	}

	void IOEngine::onIOUnListen(Packet * packet)
	{

	}

	void IOEngine::onIOAccept(Packet * packet)
	{
		IONotify* notify = dynamic_cast<IONotify*>(packet);

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