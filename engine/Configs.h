#pragma once
#include "core/Configs.h"

namespace engine
{
	using namespace core;

	enum EPacketType
	{
		PacketListen		=	1,
		PacketUnlisten		=	2,
		PacketAccept		=	3,
		PacketConnect		=	4,
		PacketDisCon		=	5,
		PacketClose			=	6,
		PacketMsg			=	7,
	};
}