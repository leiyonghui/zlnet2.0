#pragma once
#include "core/Configs.h"
#include "network/Configs.h"

namespace engine
{
	using namespace core;
	using net::EPROTOCOL;

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