#pragma once

#include "core/Configs.h"
#include "core/NonCopyable.h"
#include "core/ObjectPools.h"
#include "core/Common.h"
#include "core/Timers.h"
#include "core/KeyPool.h"

#ifdef _WIN32
#include <winsock2.h>
#endif // _WIN32

namespace network
{
	using namespace core;
	using namespace core::types;
	using namespace core::logs;

	const int32 MAX_OBJECT_SIZE = 1 << 16;
	using ObjecKeyPool = KeyPool<16>;

	class IOProtocol;
	USING_SHARED_PTR(IOProtocol);
	class IOObject;
	USING_SHARED_PTR(IOObject);
	class TcpListener;
	USING_SHARED_PTR(TcpListener);
	class TcpConnection;
	USING_SHARED_PTR(TcpConnection);
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
		IO_OBJECT_LISTENER,
		IO_OBJECT_CONNECTOR,
		IO_OBJECT_CONNECTION,
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
	};

#ifdef __linux
	using SOCKET = int;
#define INVALID_SOCKET -1
#endif // __linux

}

#include "Common.h"