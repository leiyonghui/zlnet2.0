#pragma once
#include "Configs.h"

namespace network
{
	class IOEvent
	{
	public:
		IOEvent(IOEventType type, uint32 key): _type(type), _key(key)
		{

		}
		virtual ~IOEvent() = default;
		IOEventType getType() const { return _type; }
		uint32 getKey() const { return _key; }
	protected:
		IOEventType _type;
		uint32 _key;
	};
	USING_SHARED_PTR(IOEvent);
}