#pragma once
#include "Configs.h"
#include "IOObject.h"

namespace network
{
	class CPoller
	{
	public:
		CPoller();
		virtual ~CPoller() = default;

		virtual void registerReadHandler(const IOObjectPtr& object) = 0;
		virtual void registerWriteHandler(const IOObjectPtr& object) = 0;
		virtual void deregisterReadhandler(const IOObjectPtr& object) = 0;
		virtual void deregisterWritehandler(const IOObjectPtr& object) = 0;
		virtual void deregisterObject(const IOObjectPtr& object) = 0;
		virtual void poll() = 0;

		bool hasObject(const IOObjectPtr& object) const;
		bool hasObject(uint32 key) const;
	protected:
		IOObjectPtr getObject(uint32 key) const;

		std::vector<IOObjectPtr> _handlers;
	};
}