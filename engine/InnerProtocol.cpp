#include "InnerProtocol.h"

namespace engine
{
	InnerProtocol::InnerProtocol(net::EPROTOCOL type): Protocol(type)
	{

	}

	net::IOProtocolPtr InnerProtocol::create() const
	{
		auto protocol = std::make_shared<InnerProtocol>(net::EPROTO_TCP);
		protocol->setQueue(_msgqueue);
		return protocol;
	}

	void InnerProtocol::onUnserialize(net::IStream* buffer)
	{

	}

	void InnerProtocol::onSerialize(net::IOEvent* event, net::IBuffer* buffer)
	{

	}
	
}
