#include "InnerProtocol.h"

namespace engine
{
    void InnerProtocol::onAwake(net::EPROTOCOL pType)
    {
		Protocol::onAwake(pType);
    }

    void InnerProtocol::onRecycle()
    {
		Protocol::onRecycle();
    }

    net::IOProtocolPtr InnerProtocol::create() const
	{
		auto protocol = CObjectPool<InnerProtocol>::Instance()->create(net::EPROTO_TCP);
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
