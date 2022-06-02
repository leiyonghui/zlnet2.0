#pragma once
#include "Configs.h"

namespace net
{
	class IOEvent
	{
	public:
		IOEvent(EIOEventType type, uint32 key): _type(type), _key(key)
		{

		}
		virtual ~IOEvent() = default;
		EIOEventType getType() const { return _type; }
		uint32 getKey() const { return _key; }
	protected:
		EIOEventType _type;
		uint32 _key;
	};
	USING_SHARED_PTR(IOEvent);
}