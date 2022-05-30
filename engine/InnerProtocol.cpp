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

	void InnerProtocol::onSerialize(network::IStream* buffer)
	{
	}

	void InnerProtocol::onUnserialize(network::IOEvent* event, network::IBuffer* buffer)
	{
	}
}
