#pragma once
#include "Configs.h"

namespace engine
{
	class Packet
	{
	public:
		Packet(int32 type, uint32 uid):_type(type),_uid(uid){}
		virtual ~Packet() = default;

		uint32 getUid() const { return _uid; };

	protected:
		int32 _type;
		uint32 _uid;
	};
}