#pragma once
#include "Configs.h"

namespace engine
{
	class Packet
	{
	public:
		Packet(EPacketType type, uint32 uid):_type(type),_uid(uid){}
		virtual ~Packet() = default;

		EPacketType getType() const { return _type; }
		uint32 getUid() const { return _uid; };
	protected:
		EPacketType _type;
		uint32 _uid;
	};


}