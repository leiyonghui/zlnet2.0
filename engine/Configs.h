#pragma once
#include "core/Configs.h"
#include "network/Configs.h"

namespace engine
{
	using namespace core;
	using net::EPROTOCOL;

	class Protocol;
	USING_SHARED_PTR(Protocol);
	class Packet;
	USING_SHARED_PTR(Packet);
	class IONotify;
	USING_SHARED_PTR(IONotify);
	class IOPacket;
	USING_SHARED_PTR(IOPacket);

	enum EPacketType
	{
		PacketListen	=	1,
		PacketUnlisten,
		PacketAccept,
		PacketConnect,
		PacketDisconnect,
		PacketClose,	
		PacketMsg,
	};
}