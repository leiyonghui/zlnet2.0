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
		uint32 listen(uint16 port, IOProtocolPtr protocol);//multi-thread
		uint32 connect(const std::string& ip, uint16 port, IOProtocolPtr protocol);//multi-thread
		void close(uint32 key);//multi-thread

		template<class UserData>
		void send(IOEventData<UserData>* event)//multi-thread
		{
			pushEvent(static_cast<IOEvent*>(event));
		}

		uint32 popKey();//multi-thread
		void pushKey(uint32 key);//multi-thread
	protected:
		void addObject(const IOObjectPtr& object);
		void removeObject(uint32 key);
		IOObjectPtr getObject(uint32 key) const;

		void init();
		void loop();
		void pushEvent(IOEvent* event);
		void dispatchProcess(IOEvent* event);
		void processObjectEvent(IOEvent* event);
		void processListen(IOListen* event);
		void processConnect(IOEConnect* event);

		void defaultErrorHandle(const IOObjectPtr& object);

		void handleTcpAccept(const IOObjectPtr& object);
		void handleTcpConError(const IOObjectPtr& object);
		void handleTcpConRead(const IOObjectPtr& object);
		void handleTcpConWrite(const IOObjectPtr& object);
		void closeTcpCon(const TcpConnectionPtr& con);
		void processTcpObjectEvent(const IOObjectPtr& object, IOEvent* event);
		void tcpListen(int16 port, const IOProtocolPtr& protocol);
		void tcpConnect(const std::string& ip, uint16 port, const IOProtocolPtr& protocol);
	private:
		bool _isStart;
		ObjecKeyPool _keyPool;
		std::vector<IOObjectPtr> _objects;
		Queue<IOEvent*> _eventQueue;
		CPoller* _poller;
	};
}