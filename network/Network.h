#pragma once
#include "Configs.h"
#include "IOObject.h"
#include "IOObjects.h"
#include "IOProtocol.h"
#include "IOEvents.h"
#include "Poller.h"
#include "core/Queue.h"

namespace net
{
	class CNetwork
	{
	public:
		CNetwork();
		virtual ~CNetwork();
	public:
		void start();
		void stop();
		bool isStart();

		uint32 listen(uint16 port, IOProtocolPtr protocol);//multi-thread
		uint32 connect(const std::string& ip, uint16 port, IOProtocolPtr protocol);//multi-thread
		void close(uint32 key, int32 second);//multi-thread

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
		void onQuit();
		void onTimer1000ms();
		void pushEvent(IOEvent* event);
		void dispatchProcess(IOEvent* event);
		void processDataEvent(IOEvent* event);
		void processListen(IOListen* event);
		void processConnect(IOEConnect* event);
		void processClose(IOClose* event);

		void defaultErrorHandle(const IOObjectPtr& object);

		void handleTcpAccept(const IOObjectPtr& object);
		void handleTcpConError(const IOObjectPtr& object);
		void handleTcpConRead(const IOObjectPtr& object);
		void handleTcpConWrite(const IOObjectPtr& object);
		void handleTcpConnectWrite(const IOObjectPtr& object);
		void handleTcpConnectError(const IOObjectPtr& object);
		void removeTcpListen(const TcpListenerPtr& listener);
		void removeTcpCon(const ConnectionPtr& con);
		void tcpListen(int16 port, const IOProtocolPtr& protocol);
		void tcpSend(const IOObjectPtr& object, IOEvent* event);
		void tcpConnect(const std::string& ip, uint16 port, const IOProtocolPtr& protocol);
		void tcpConnectError(const TcpConnectorPtr& connect);
		void tcpClose(uint32 key, uint32 second);
		void tcpClose(const TcpConnectionPtr& con, int32 second);
		void tcpClose(const TcpConnectorPtr& con, int32 second);
		void tcpClose(const TcpListenerPtr& listener);
	private:
		bool _isStart;
		std::atomic_bool _isStop;
		ObjecKeyPool _keyPool;
		std::vector<IOObjectPtr> _objects;
		Queue<IOEvent*> _eventQueue;
		CPoller* _poller;
		IScheduler* _shceduler;
		TimerHander* _timerHandler;
		int64 _lastclock;
	};
}