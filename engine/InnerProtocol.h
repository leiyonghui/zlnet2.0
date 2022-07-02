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

		virtual bool onUnserialize(net::IIOBuffer* buffer) override;

		virtual bool onSerialize(net::IOEvent* event, net::IBuffer* buffer) override;
	};
	USING_SHARED_PTR(InnerProtocol);
}