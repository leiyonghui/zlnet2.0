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
		IStream* getInputBuffer() { return _inputBuffer; };
		IStream* getOutBuffer() { return _outBuffer; };
		bool isWriting() const { return _isWriting; }
		void setWriting(bool is) { _isWriting = is; }
		bool enable() { return _state == CONNECTED || _state == DISCONNECTING; }
	protected:
		EConnectState _state;
		IStream* _inputBuffer;
		IStream* _outBuffer;
		bool _isWriting;
	};
	USING_SHARED_PTR(Connection);
}