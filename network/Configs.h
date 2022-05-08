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

	class IOObject;
	USING_SHARED_PTR(IOObject);

	enum EPROTOCOL
	{
		EPROTO_TCP = 0,
		EPROTO_UDP = 1,
		EPROTO_KCP = 2,
	};

	enum IO_OBJECT_TYPE
	{
		IO_OBJECT_TYPE_NONE = 0,
		IO_OBJECT_TYPE_LISTENER,
		IO_OBJECT_TYPE_CONNECTOR,
		IO_OBJECT_TYPE_CONNECTION,
	};

	enum EConnectState
	{
		EConnecting = 1,		//正在连接
		EConnected = 2,			//已连接
		EDisconnecting = 3,		//正在关闭
		EDisconnected = 4,		//已关闭
	};

	enum IOEventType
	{
		IO_EVENT_NONE = 0,
		IO_EVENT_LISTEN,
		IO_EVENT_ACCEPT,
		IO_EVENT_CONNECT,
		IO_EVENT_RW,
		IO_EVENT_DATA,
	};

#ifdef __linux
	using SOCKET = int;
#define INVALID_SOCKET -1
#endif // __linux

}

#include "Common.h"