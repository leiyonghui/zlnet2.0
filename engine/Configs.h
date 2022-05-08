#pragma once
#include "core/Configs.h"

namespace core
{
	enum EMsgEvent
	{
		EIOListen		=	1,
		EIOUnlisten		=	2,
		EIOAccept		=	3,
		EIOConnect		=	4,
		EIODisCon		=	5,
		EIOClose		=	6,
		EIOPacket		=	7,
	};
}