#pragma once
#include "IOObject.h"
#include "IIOBuffer.h"

namespace net
{
	class Connection : public IOObject, public TimerImpl<Connection>
	{
	public:
		Connection(EIOOBJECT_TYPE type);
		~Connection();
		void onAwake(const IOProtocolPtr& protocol, CEndPointUnPtr endPoint);
		void onRecycle();

		void setState(EConnectState state) { _state = state; };
		EConnectState getState() const { return _state; };
		IIOBuffer* getInputBuffer() { return _inputBuffer; };
		IIOBuffer* getOutBuffer() { return _outBuffer; };
		bool isWriting() const { return _isWriting; }
		void setWriting(bool is) { _isWriting = is; }
		bool enableSend() { return _state == CONNECTED; }
		bool enableWrite() { return _state == CONNECTED || _state == DISCONNECTING; }
		bool enableRead() { return _state == CONNECTED || _state == DISCONNECTING; }
	protected:
		EConnectState _state;
		IIOBuffer* _inputBuffer;
		IIOBuffer* _outBuffer;
		bool _isWriting;
	};
	USING_SHARED_PTR(Connection);
}