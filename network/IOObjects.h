#pragma once
#include "IOObject.h"

namespace core
{
	class CRingBuff;
}

namespace network
{
	using core::CRingBuff;

	class Connection : public IOObject
	{
	public:
		Connection();
		void onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint);
		void onRecycle();

		void setState(EConnectState state) { _state = state; };
		EConnectState getState() const { return _state; };
		CRingBuff* getInputBuff() { return _inputBuff; };
		CRingBuff* getOutBuff() { return _outBuff; };
	protected:
		EConnectState _state;
		CRingBuff* _inputBuff;
		CRingBuff* _outBuff;
	};
}