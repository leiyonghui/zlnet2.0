#pragma once
#include "Configs.h"

namespace engine
{
	class PacketHandler
	{
	public:
		virtual ~PacketHandler() = default;

		virtual void onPacket(const PacketPtr& packet) = 0;
	};

	class CallbackHandler : public PacketHandler
	{
	public:

	private:
		uint32 _uid;
		uint32 _cbId;
		uint32 _cmd;
	};

	class MessageHandler : public PacketHandler
	{
	public:

		virtual void onPacket(const PacketPtr& packet) override;

		//virtual void onMessage(CMessageContext& context) = 0;
	private:

	};

	class MessageHandlerBinding : public MessageHandler
	{
	public:

	private:
		std::function<void()> _func;
	};
}