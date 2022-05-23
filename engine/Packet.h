#pragma once
#include "Configs.h"

namespace engine
{
	class Packet
	{
	public:
		Packet(EPacketType type):_type(type){}
		virtual ~Packet() = default;
	protected:
		EPacketType _type;
	};
}