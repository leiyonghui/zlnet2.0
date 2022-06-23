#pragma once
#include "Configs.h"
#include "core/MsgQueue.h"
#include "network/IOProtocol.h"

namespace engine
{
	class Protocol : public net::IOProtocol
	{
	public:
		Protocol();

		void onAwake(EPROTOCOL pType);
		void onRecycle();

		bool isAvailable() const;

		void setAvailable();

		void unsetAvailable();

		void setQueue(MsgQueue<PacketPtr>* queue);

		CallbackHandlerExistList& getCallbackList();

		int32 getHeartCount() const;

		void setHeartCount(int32 count);
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
		int32 _heartbeat;
		MsgQueue<PacketPtr>* _msgqueue;
		CallbackHandlerExistList _callbacklist;
	};
}