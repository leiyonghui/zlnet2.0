#pragma once
#include "core/Configs.h"

namespace engine
{
	using namespace core;
	using network::EPROTOCOL;

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