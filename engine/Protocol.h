#pragma once
#include "Configs.h"
#include "network/IOProtocol.h"
#include "core/MsgQueue.h"

namespace engine
{
	class Packet;
	class Protocol : public net::IOProtocol
	{
	public:
		Protocol(net::EPROTOCOL type);

		bool isAvailable() const;

		void setAvailable();

		void unsetAvailable();

		void setQueue(MsgQueue<Packet*>* queue);

	protected:
		void dispatchPacket(Packet* packet);

	protected:
		virtual void onListen(bool success) override;

		virtual void onUnlisten() override;

		virtual void onAccept(const net::IOProtocolPtr&) override;

		virtual void onClose() override;

		virtual void onConnect(bool dis) override;

		virtual void onDisConnect() override;

	protected:
		bool _available;
		MsgQueue<Packet*>* _msgqueue;
	};
	USING_SHARED_PTR(Protocol);
}