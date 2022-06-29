#pragma once
#include "Configs.h"
//#include "IOObject.h"
//#include "IOObjects.h"
//#include "IOProtocol.h"
#include "IOEvents.h"
//#include "Poller.h"
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
		bool isRuning();

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

		void run();
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
		void processClear(IOEventClear* event);

		void defaultErrorHandle(const IOObjectPtr& object);

		void handleTcpAccept(const IOObjectPtr& object);
		void handleTcpConError(const IOObjectPtr& object);
		void handleTcpConRead(const IOObjectPtr& object);
		void handleTcpConWrite(const IOObjectPtr& object);
		void handleTcpConnectWrite(const IOObjectPtr& object);
		void handleTcpConnectError(const IOObjectPtr& object);
		void tcpListen(int16 port, const IOProtocolPtr& protocol);
		void tcpSend(const IOObjectPtr& object, IOEvent* event);
		void tcpConnect(const std::string& ip, uint16 port, const IOProtocolPtr& protocol);
		void tcpConnectError(const TcpConnectorPtr& connect);
		void tcpClose(uint32 key, bool force);
		void tcpCloseCon(const ConnectionPtr& con, bool force);
		void tcpCloseConnection(const TcpConnectionPtr& con, bool force);
		void tcpCloseConnector(const TcpConnectorPtr& con, bool force);
		void tcpCloseListen(const TcpListenerPtr& listener);
		void removeTcpListen(const TcpListenerPtr& listener);
		void removeTcpCon(const ConnectionPtr& con);

	private:
		bool _isRuning;
		std::atomic_bool _isStop;
		ObjecKeyPool _keyPool;
		std::vector<IOObjectPtr> _objects;
		Queue<IOEvent*> _eventQueue;
		class CPoller* _poller;
		IScheduler* _shceduler;
		TimerHander* _timerHandler;
		int64 _lastclock;
		std::thread _thr;
	};
}