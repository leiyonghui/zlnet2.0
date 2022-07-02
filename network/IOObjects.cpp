#include "RingBuffer.h"
#include "IOObjects.h"

namespace net
{
	Connection::Connection(EIOOBJECT_TYPE type):IOObject(type), _inputBuffer(new CRingBuffer(1024)), _outBuffer(new CRingBuffer(1024))
	{

	}

	Connection::~Connection()
	{
		delete _inputBuffer;
		delete _outBuffer;
	}

	void Connection::onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint)
	{
		IOObject::onAwake(protocol, std::move(endPoint));
		_state = DISCONNECTED;
		_isWriting = false;
	}

	void Connection::onRecycle()
	{
		IOObject::onRecycle();
		_inputBuffer->clear();
		_outBuffer->clear();
		TimerImpl<Connection>::reset();
	}
}