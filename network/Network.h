#pragma once
#include "Configs.h"
#include "IOObjects.h"
#include "IOProtocol.h"
#include "IOEvents.h"
#include "core/Queue.h"

namespace network
{
	class CNetwork
	{
	public:
		CNetwork();
		virtual ~CNetwork();
	public:
		uint32 popKey();
		void pushKey(uint32 key);
		void start();
		uint32 listen(uint16 port, IOProtocolPtr protocol);
		uint32 connect(const std::string& ip, uint16 port, IOProtocolPtr protocol);
		uint32 close(uint32 key);
	protected:
		bool addObject(const IOObjectPtr& object);
		IOObjectPtr getObject(uint32 key) const;
	private:
		void init();
		void loop();
		void dispatchProcess(const IOEventPtr& event);
		void pushEvent(const IOEventPtr& event);
		void processListen(const IOEventPtr& event);

		void handPollTcpAccept(const IOObjectPtr& object);
	private:
		ObjecKeyPool _keyPool;
		std::vector<IOObjectPtr> _objects;
		Queue<IOEventPtr> _eventQueue;
		bool _isStop;
	};
}