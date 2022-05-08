#pragma once
#include "IOProtocol.h"
#include "Endpoint.h"

namespace network
{
	using ErrorCallback = std::function<void(const IOObjectPtr&)>;
	using ReadCallback = std::function<void(const IOObjectPtr&)>;
	using WriteCallback = std::function<void(const IOObjectPtr&)>;

	class IOObject : public CNoncopyable
	{
	public:
		IOObject(IO_OBJECT_TYPE type, uint32 key, IOProtocolPtr protocl): _type(type), _key(key), _protocol(protocl), _events(0)
		{

		}

		IO_OBJECT_TYPE type() const { return _type; }
		uint32 key() const { return _key; }
		IOProtocolPtr getProtocol() const { return _protocol; }

		int32 getEvents() const { return _events; }
		void updateEvents(int32 events) { _events = events; }
		void setErrorCallback(ErrorCallback&& callback) { _errorCallback = std::move(callback); }
		void setReadCallback(ReadCallback&& callback) { _readCalllback = std::move(callback); }
		void setWriteCallbac(WriteCallback&& callback) { _writeCallback = std::move(callback); }

		const ErrorCallback& getErrorCallback() const { return _errorCallback; }
		const ReadCallback& getReadCallback() const { return _readCalllback; }
		const WriteCallback& getWriteCallback() const { return _writeCallback; }
		SOCKET getSocket() const { return _endpoint ? _endpoint->getSocket() : INVALID_SOCKET; }
	protected:
		IO_OBJECT_TYPE _type;
		uint32 _key;
		IOProtocolPtr _protocol;

		int32 _events;
		ErrorCallback	_errorCallback;
		ReadCallback	_readCalllback;
		WriteCallback	_writeCallback;
		CEndPointUnPtr _endpoint;
	};
}