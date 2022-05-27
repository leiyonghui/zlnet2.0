#pragma once
#include "Configs.h"
#include "network/IOProtocol.h"
#include "core/MsgQueue.h"

namespace engine
{
	class Protocol : public network::IOProtocol
	{
	public:
		bool isAvailable() const;

		void setAvailable();

		void unsetAvailable();

		void setQueue(MsgQueue<Packet*>* queue);

	protected:
		void dispatchPacket(class Packet* packet);

	protected:
		virtual void onListen(bool success) override;

		virtual void onUnlisten() override;

		virtual void onAccept(network::IOProtocolPtr&) override;

		virtual void onClose() override;

		virtual void onConnect(bool dis) override;

		virtual void onDisConnect() override;

	protected:
		bool _available;
		MsgQueue<Packet*>* _msgqueue;
	};
	USING_SHARED_PTR(Protocol);
}