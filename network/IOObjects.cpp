#include "IOObjects.h"
#include "core/RingBuff.h"

namespace network
{
	Connection::Connection():IOObject(IO_OBJECT_CONNECTION), _inputBuff(new CRingBuff(1024)), _outBuff(new CRingBuff(1024))
	{

	}

	void Connection::onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint)
	{
		IOObject::onAwake(protocol, std::move(endPoint));
		_state = DISCONNECTED;
	}

	void Connection::onRecycle()
	{
		IOObject::onRecycle();
		_inputBuff->clear();
		_outBuff->clear();
	}
}