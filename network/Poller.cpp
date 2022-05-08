#include "Poller.h"

namespace network
{
	CPoller::CPoller()
	{
		_handlers.resize(MAX_OBJECT_SIZE);
	}

	bool CPoller::hasObject(const IOObjectPtr& object) const
	{
		return getObject(object->key()) == object;
	}

	bool CPoller::hasObject(uint32 key) const
	{
		return getObject(key) != nullptr;
	}

	IOObjectPtr CPoller::getObject(uint32 key) const
	{
		auto index = ObjecKeyPool::index(key);
		auto& object = _handlers[index];
		if (object && object->key() == key)
		{
			return object;
		}
		return nullptr;
	}
}