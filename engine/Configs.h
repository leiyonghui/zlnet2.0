#pragma once
#include "core/Configs.h"
#include "network/Configs.h"

namespace engine
{
	using namespace core;
	using network::EPROTOCOL;

	class Protocol;
	USING_SHARED_PTR(Protocol);
	class Packet;
	USING_SHARED_PTR(Packet);
	class IONotify;
	USING_SHARED_PTR(IONotify);
	class IOPacket;
	USING_SHARED_PTR(IOPacket);
	class PacketHandler;
	USING_SHARED_PTR(PacketHandler);
	class CallbackHandler;
	USING_SHARED_PTR(CallbackHandler);

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