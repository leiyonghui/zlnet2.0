#pragma once
#include "IOObject.h"

namespace net
{
	class CPoller
	{
	public:
		CPoller() = default;
		virtual ~CPoller() = default;

		virtual void registerReadHandler(const IOObjectPtr& object);
		virtual void registerWriteHandler(const IOObjectPtr& object);
		virtual void deregisterReadHandler(const IOObjectPtr& object);
		virtual void deregisterWriteHandler(const IOObjectPtr& object);
		virtual void deregisterObject(const IOObjectPtr& object);
		virtual void poll(const std::vector<IOObjectPtr>& objects, int32 millisecond) = 0;

		virtual void updateObject(const IOObjectPtr& object, int32 events) = 0;
		virtual void removeObject(const IOObjectPtr& object) = 0;
	};
	USING_SHARED_PTR(CPoller);
}