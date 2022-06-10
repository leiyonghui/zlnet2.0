#include "Messages.h"

namespace engine
{
	std::map<int32, MessageCreator>& MessageFactory::getMessageCreators()
	{
		static std::map<int32, MessageCreator> messageCreators;
		return messageCreators;
	}

	void MessageFactory::registerMessage(int32 id, MessageCreator creator)
	{
		auto& creators = getMessageCreators();
		if (creators.find(id) != creators.end())
		{
			core_log_error("register message dup id", id);
			assert(false);
		}
		creators[id] = creator;
	}

	IMessagePtr MessageFactory::Create(int32 id)
	{
		auto& creators = getMessageCreators();
		auto iter = creators.find(id);
		if (iter == creators.end())
		{
			core_log_error("message creator null", id);
			return nullptr;
		}
		return iter->second();
	}

	SerializeMessage::SerializeMessage():_id(0), _buffer(128)
	{

	}

	SerializeMessage::SerializeMessage(int32 id, const net::Buffer& buffer):_id(id), _buffer(buffer)
	{

	}

	SerializeMessage::SerializeMessage(const IMessagePtr& message):_id(message->identity()), _buffer(128)
	{
		net::BufferWriter writer(&_buffer);
		message->serialize(writer);
	}

	SerializeMessage::SerializeMessage(int32 id, const char* ptr, uint32 len):_id(id), _buffer(len)
	{
		_buffer.write(0, ptr, len);
	}

    void SerializeMessage::serialize(net::BufferReader& archive)
    {
		_buffer.clear();
		archive.readBytes(_buffer.write(0, archive.bytesLeft()), archive.bytesLeft());
    }

    void SerializeMessage::serialize(net::BufferWriter& archive) const
    {
		archive.writeBytes(_buffer.read(0, _buffer.size()), _buffer.size());
    }
}