#include "PacketHandlers.h"
#include "IOPacket.h"
#include "Messages.h"

namespace engine
{
	PacketHandlerImpl::PacketHandlerImpl(const std::function<void(const IOPacketPtr& packet)>& func):_func(func)
	{
	}
	void PacketHandlerImpl::onPacket(const IOPacketPtr& packet)
	{
		try
		{
			_func(packet);
		}
		catch (std::exception& ex)
		{
			core_log_error("PacketHandlerImpl on Packet", ex.what());
		}
	}

	CallbackHandlerImpl::CallbackHandlerImpl(const std::function<void(const IOPacketPtr& packet)>& func):_func(func)
	{

	}

	void CallbackHandlerImpl::onPacket(const IOPacketPtr& packet)
	{
		try
		{
			_func(packet);
		}
		catch (std::exception& ex)
		{
			core_log_error("callback on packet", ex.what());
		}
	}

	void MessageHandler::onPacket(const IOPacketPtr& packet)
	{
		IMessagePtr message = onMessageConvert(packet->getMessage());
		CMessageContext context(packet->getUid(), packet->getCommand(), message);
		onMessage(context);		
	}

	IMessagePtr MessageHandler::onMessageConvert(const IMessagePtr& message)
	{
		if (SerializeMessagePtr msg = std::dynamic_pointer_cast<SerializeMessage>(message))
		{
			IMessagePtr newMsg = MessageFactory::Create(msg->identity());
			if (newMsg)
			{
				net::BufferReader reader(msg->getBuffer());
				newMsg->serialize(reader);
			}
			return newMsg;
		}
		else
		{
			return message;
		}
	}

	MessageHandlerBinding::MessageHandlerBinding(const std::function<void(CMessageContext&)>& func):_func(func)
	{

	}

	void MessageHandlerBinding::onMessage(CMessageContext& context)
	{
		try
		{
			_func(context);
		}
		catch (std::exception& e)
		{
			core_log_error("onMessage", e.what());
		}
	}	

}