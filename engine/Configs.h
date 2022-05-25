#pragma once
#include "core/Configs.h"

namespace engine
{
	using namespace core;

	enum EPacketType
	{
		PacketListen	=	1,
		PacketUnlisten,
		PacketAccept,
		PacketConnect,
		PacketClose,	
		PacketMsg,
	};
}