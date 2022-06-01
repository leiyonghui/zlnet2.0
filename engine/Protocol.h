#pragma once
#include "Configs.h"
#include "network/IOProtocol.h"
#include "core/MsgQueue.h"

namespace engine
{
	class Protocol : public network::IOProtocol
	{
	public:
		Protocol(network::EPROTOCOL type);

		bool isAvailable() const;

		void setAvailable();

		void unsetAvailable();

		void setQueue(MsgQueue<PacketPtr>* queue);

	protected:
		void dispatchPacket(PacketPtr packet);

	protected:
		virtual void onListen(bool success) override;

		virtual void onUnlisten() override;

		virtual void onAccept(const network::IOProtocolPtr&) override;

		virtual void onClose() override;

		virtual void onConnect(bool dis) override;

		virtual void onDisConnect() override;

	protected:
		bool _available;
		MsgQueue<PacketPtr>* _msgqueue;
	};
}