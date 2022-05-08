#pragma once
#include "Configs.h"

namespace network
{
	typedef std::shared_ptr<class CConnection> CConnectionPtr;

	typedef std::function<void(class CAddress)> onDisListenCallback;

	typedef std::function<void(CConnectionPtr connection)> onNewConnectionCallback;

	typedef std::function<void(CConnectionPtr connection, class CRingBuff* ringBuff)> onConnectionInputCallback;

	typedef std::function<void(CConnectionPtr connection)> onConnectionCloseCallback;

	typedef std::function<void(CAddress)> onTimeoutCallback;
}
