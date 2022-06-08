#pragma once
#include "Configs.h"
#include "network/IOProtocol.h"
#include "core/MsgQueue.h"
#include "PacketHandlers.h"

namespace engine
{
	class Protocol : public net::IOProtocol
	{
	public:
		void onAwake(EPROTOCOL pType);
		void onRecycle();

		bool isAvailable() const;

		void setAvailable();

		void unsetAvailable();

		void setQueue(MsgQueue<PacketPtr>* queue);

		CallbackHandlerExistList& getCallbackList();
	protected:
		void dispatchPacket(PacketPtr packet);

	protected:
		virtual void onListen(bool success) override;

		virtual void onUnlisten() override;

		virtual void onAccept(const net::IOProtocolPtr&) override;

		virtual void onClose() override;

		virtual void onConnect(bool dis) override;

		virtual void onDisConnect() override;

	protected:
		bool _available;
		MsgQueue<PacketPtr>* _msgqueue;
		CallbackHandlerExistList _callbacklist;
	};
}