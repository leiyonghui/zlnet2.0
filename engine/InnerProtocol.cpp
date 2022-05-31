#include "InnerProtocol.h"

namespace engine
{
	InnerProtocol::InnerProtocol(network::EPROTOCOL type): Protocol(type)
	{

	}

	network::IOProtocolPtr InnerProtocol::create() const
	{
		auto protocol = std::make_shared<InnerProtocol>(network::EPROTO_TCP);
		protocol->setQueue(_msgqueue);
		return protocol;
	}

	void InnerProtocol::onUnserialize(network::IStream* buffer)
	{

	}

	void InnerProtocol::onSerialize(network::IOEvent* event, network::IBuffer* buffer)
	{

	}
	
}
