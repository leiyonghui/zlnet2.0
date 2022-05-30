#include "InnerProtocol.h"

namespace engine
{
	InnerProtocol::InnerProtocol(network::EPROTOCOL type): Protocol(type)
	{

	}

	network::IOProtocolPtr InnerProtocol::create() const
	{
		return network::IOProtocolPtr();
	}

	void InnerProtocol::onUnserialize(network::IStream* buffer)
	{

	}

	void InnerProtocol::onSerialize(network::IOEvent* event, network::IBuffer* buffer)
	{

	}
	
}
