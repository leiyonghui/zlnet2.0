#pragma once

#include "core/Configs.h"
#include "core/NonCopyable.h"
#include "core/ObjectPools.h"
#include "core/Common.h"
#include "core/Timers.h"
#include "core/KeyPool.h"

namespace net
{
	using namespace core;
	using namespace core::types;

	using ObjecKeyPool = KeyPool<16>;
	const int32 MAX_OBJECT_SIZE = ObjecKeyPool::key_maxnum;

	class IOProtocol;
	USING_SHARED_PTR(IOProtocol);
	class IOObject;
	USING_SHARED_PTR(IOObject);
	class TcpListener;
	USING_SHARED_PTR(TcpListener);
	class Connection;
	USING_SHARED_PTR(Connection);
	class TcpSession;
	USING_SHARED_PTR(TcpSession);
	class TcpConnector;
	USING_SHARED_PTR(TcpConnector);

	enum EPROTOCOL
	{
		EPROTO_TCP = 0,
		EPROTO_UDP = 1,
		EPROTO_KCP = 2,
	};

	enum EIOOBJECT_TYPE
	{
		IO_OBJECT_WAKEUP,
		IO_OBJECT_LISTENER,
		IO_OBJECT_CONNECTOR,
		IO_OBJECT_SESSION,
	};

	enum EConnectState
	{
		DISCONNECTED = 0,		//已关闭
		CONNECTING = 1,			//正在连接
		CONNECTED = 2,			//已连接
		DISCONNECTING = 3,		//正在关闭
	};

	enum EIOEventType
	{
		IO_EVENT_LISTEN,
		IO_EVENT_CONNECT,
		IO_EVENT_DATA,
		IO_EVENT_CLOSE,
		IO_EVENT_CLEAR,
	};

	enum EIOEvent
	{
		READ_EVENT = 1,
		WRITE_EVENT = 2,
		RDWR_EVENT = 1 | 2,
	};

#ifdef __linux
	using SOCKET = int;
#define INVALID_SOCKET -1
#endif // __linux

}
