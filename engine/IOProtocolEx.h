#pragma once
#include "Configs.h"
#include "network/IOProtocol.h"

namespace engine
{
	class IOProtocolEx : public network::IOProtocol
	{
	public:
		virtual void onListen() override;

		virtual void onUnlisten() override;

		virtual void onAccept() override;

		virtual void onClose() override;

		virtual void onConnect() override;

		virtual void onDisconnect() override;
	};
	USING_SHARED_PTR(IOProtocolEx);
}