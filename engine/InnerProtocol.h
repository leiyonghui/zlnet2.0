#pragma once
#include "Protocol.h"

namespace engine
{
	class InnerProtocol : public Protocol
	{
	public:
		void onAwake(net::EPROTOCOL pType);

		void onRecycle();

		virtual net::IOProtocolPtr create() const override;

		virtual void onUnserialize(net::IStream* buffer) override;

		virtual void onSerialize(net::IOEvent* event, net::IBuffer* buffer) override;
	};
	USING_SHARED_PTR(InnerProtocol);
}