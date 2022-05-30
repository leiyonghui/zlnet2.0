#pragma once
#include "Protocol.h"

namespace engine
{
	class InnerProtocol : public Protocol
	{
	public:
		InnerProtocol(network::EPROTOCOL type);

		virtual network::IOProtocolPtr create() const override;

		virtual void onUnserialize(network::IStream* buffer) override;

		virtual void onSerialize(network::IOEvent* event, network::IBuffer* buffer) override;
	};
	USING_SHARED_PTR(InnerProtocol);
}