#pragma once
#include "IOProtocol.h"
#include "Endpoint.h"
#include "core/TimerImpl.h"

namespace net
{
	using ErrorCallback = std::function<void(const IOObjectPtr&)>;
	using ReadCallback = std::function<void(const IOObjectPtr&)>;
	using WriteCallback = std::function<void(const IOObjectPtr&)>;

	class IOObject : public core::CPoolObject, public CNoncopyable, public std::enable_shared_from_this<IOObject>
	{
	public:
		IOObject(EIOOBJECT_TYPE type);
		IOObject( EIOOBJECT_TYPE type, uint32 key, CEndPointUnPtr&& endPoint);
		virtual ~IOObject() = default;

		void onAwake(const IOProtocolPtr& protocl, CEndPointUnPtr endPoint);
		void onRecycle();

		EIOOBJECT_TYPE getType() const { return _type; }
		uint32 getKey() const { return _key; }
		IOProtocolPtr getProtocol() const { return _protocol; }

		void setGroup(int32 group) { _group = group; }
		int32 getGroup() const { return _group; }
		int32 getEvents() const { return _events; }
		void updateEvents(int32 events) { _events = events; }
		bool hasReading() const { return READ_EVENT & _events; }
		bool hasWriting() const { return WRITE_EVENT & _events; }
		void setErrorCallback(ErrorCallback&& callback) { _errorCallback = std::move(callback); }
		void setReadCallback(ReadCallback&& callback) { _readCalllback = std::move(callback); }
		void setWriteCallback(WriteCallback&& callback) { _writeCallback = std::move(callback); }

		const ErrorCallback& getErrorCallback() const { return _errorCallback; }
		const ReadCallback& getReadCallback() const { return _readCalllback; }
		const WriteCallback& getWriteCallback() const { return _writeCallback; }
		SOCKET getSocket() const { return _endpoint ? _endpoint->getSocket() : INVALID_SOCKET; }
		CEndPoint* getEndPoint() { return _endpoint.get(); }
	protected:
		EIOOBJECT_TYPE _type;
		uint32 _key;
		int32 _group;
		IOProtocolPtr _protocol;

		int32 _events;
		ErrorCallback	_errorCallback;
		ReadCallback	_readCalllback;
		WriteCallback	_writeCallback;
		CEndPointUnPtr _endpoint;
	};
}