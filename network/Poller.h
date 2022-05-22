#pragma once
#include "IOObject.h"

namespace network
{
	class CPoller
	{
	public:
		CPoller() = default;
		virtual ~CPoller() = default;

		virtual void registerReadHandler(const IOObjectPtr& object) = 0;
		virtual void registerWriteHandler(const IOObjectPtr& object) = 0;
		virtual void deregisterReadHandler(const IOObjectPtr& object) = 0;
		virtual void deregisterWriteHandler(const IOObjectPtr& object) = 0;
		virtual void deregisterObject(const IOObjectPtr& object) = 0;
		virtual void poll(const std::vector<IOObjectPtr>& objects, int32 millisecond) = 0;
	};
	USING_SHARED_PTR(CPoller);
}