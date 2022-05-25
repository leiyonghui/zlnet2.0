#pragma once
#include "Configs.h"
#include "network/IOProtocol.h"

namespace engine
{
	class IOProtocol : public network::IOProtocol
	{
	public:
		bool isAvailable() const;

		void setAvailable();

		void unsetAvailable();

	protected:
		void dispatchPacket(Packet* packet);

	protected:
		virtual void onListen() override;

		virtual void onUnlisten() override;

		virtual void onAccept(network::IOProtocolPtr&) override;

		virtual void onClose() override;

		virtual void onConnect(bool dis) override;

		virtual void onDisConnect() override;

	protected:
		bool _available;

	};
	USING_SHARED_PTR(IOProtocol);
}