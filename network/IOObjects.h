#pragma once
#include "IOObject.h"
#include "IStream.h"

namespace network
{

	class Connection : public IOObject
	{
	public:
		Connection();
		void onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint);
		void onRecycle();

		void setState(EConnectState state) { _state = state; };
		EConnectState getState() const { return _state; };
		IStream* getInputBuff() { return _inputBuff; };
		IStream* getOutBuff() { return _outBuff; };
	protected:
		EConnectState _state;
		IStream* _inputBuff;
		IStream* _outBuff;
	};
}