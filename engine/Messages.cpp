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
}