#pragma once
#ifdef __linux
#include "Poller.h"

namespace network
{
	class CEPoller : public CPoller
	{
	public:
		CEPoller();
		virtual ~CEPoller();
		/*virtual void registerReadHandler(const IOObjectPtr& object) override;
		virtual void registerWriteHandler(const IOObjectPtr& object) override;
		virtual void deregisterReadHandler(const IOObjectPtr& object) override;
		virtual void deregisterWriteHandler(const IOObjectPtr& object) override;
		virtual void deregisterObject(const IOObjectPtr& object) override;*/

		virtual void poll(const std::vector<IOObjectPtr>& objects, int32 millisecond) override;
	protected:
		void updateObject(const IOObjectPtr& object, int32 events);
		void removeObject(const IOObjectPtr& object);

		int32 _epfd;
		struct epoll_event* _events;
	};
}
#endif // __liunx