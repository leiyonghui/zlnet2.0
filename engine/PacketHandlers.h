#pragma once
#include "Configs.h"
#include "core/IntrusiveNode.h"
#include "Messages.h"

namespace engine
{
	class PacketHandler
	{
	public:
		virtual ~PacketHandler() = default;

		virtual void onPacket(const IOPacketPtr& packet) = 0;
	};
	USING_SHARED_PTR(PacketHandler);

	class CallbackHandler;

	class CallbackHandlerExistTag;

	class CallbackHandlerTimeoutTag;

	using CallbackHandlerExistList = IntrusiveNode<CallbackHandler, CallbackHandlerExistTag>;

	using CallbackHandlerTimeoutList = IntrusiveNode<CallbackHandler, CallbackHandlerTimeoutTag>;

	class CallbackHandler : public PacketHandler, public CallbackHandlerExistList, public CallbackHandlerTimeoutList
	{
	public:
		CallbackHandler() : _uid(0), _cbId(0), _toMs(0), _cmd(0)
		{

		}

		uint32 _uid;
		uint32 _cbId;
		int64 _toMs;
		uint32 _cmd;
	};
	USING_SHARED_PTR(CallbackHandler);

	class CallbackHandlerImpl : public CallbackHandler
	{
	public:
		CallbackHandlerImpl(const std::function<void(const IOPacketPtr& packet)>& func):_func(func){}

		virtual void onPacket(const IOPacketPtr& packet);
		
	protected:
		std::function<void(const IOPacketPtr&packet)> _func;
	};


	class MessageHandler : public PacketHandler
	{
	public:
		virtual void onPacket(const IOPacketPtr& packet) override;

	private:
		virtual IMessagePtr onMessageConvert(const IMessagePtr& message);

		virtual void onMessage(CMessageContext& context) = 0;
	};
	USING_SHARED_PTR(MessageHandler);


	class CMessageHandlerBinding : public MessageHandler
	{
	private:
		std::function<void(CMessageContext&)> _func;

	public:
		explicit CMessageHandlerBinding(const std::function<void(CMessageContext&)>& func);

		virtual void onMessage(CMessageContext& context);
	};
}