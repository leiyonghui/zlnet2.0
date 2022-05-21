#pragma once
#include "Configs.h"
#include "IOObject.h"
#include "IOProtocol.h"
#include "IOEvents.h"
#include "Poller.h"
#include "core/Queue.h"

namespace network
{
	class CNetwork
	{
	public:
		CNetwork();
		virtual ~CNetwork();
	public:
		void start();
		uint32 listen(uint16 port, IOProtocolPtr protocol);
		uint32 connect(const std::string& ip, uint16 port, IOProtocolPtr protocol);
		uint32 close(uint32 key);

		template<class UserData>
		void send(IOEventData<UserData>* event)
		{
			pushEvent(event);
		}

		uint32 popKey();
		void pushKey(uint32 key);
	protected:
		void addObject(const IOObjectPtr& object);
		void removeObject(uint32 key);
		IOObjectPtr getObject(uint32 key) const;

		void init();
		void loop();
		void dispatchProcess(IOEvent* event);
		void pushEvent(IOEvent* event);
		void processListen(IOEvent* event);
		void processData(IOEvent* event);
		void processConnect(IOEvent* event);
		void processClose(IOEvent* event);

		void defaultErrorHandle(const IOObjectPtr& object);

		void tcpListen(int16 port, const IOProtocolPtr& protocol);
		void handleTcpAccept(const IOObjectPtr& object);
		void handleTcpConError(const IOObjectPtr& object);
		void handleTcpConRead(const IOObjectPtr& object);
		void handleTcpConWrite(const IOObjectPtr& object);
		void closeTcpCon(const TcpConnectionPtr& con);

	private:
		bool _isStop;
		ObjecKeyPool _keyPool;
		std::vector<IOObjectPtr> _objects;
		Queue<IOEvent*> _eventQueue;
		CPoller* _poller;
	};
}