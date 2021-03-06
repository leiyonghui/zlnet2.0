#pragma once
#include "Configs.h"
#include "IIOBuffer.h"
#include "IOEvent.h"
#include "IBuffer.h"
#include "Address.h"

namespace net
{
	class IOProtocol : public CNoncopyable, public std::enable_shared_from_this<IOProtocol>, public CPoolObject
	{
		friend class CNetwork;
	public:
		IOProtocol();
		virtual ~IOProtocol();

		void onAwake(EPROTOCOL pType);
		void onRecycle();

		EIOOBJECT_TYPE getType() const { return _type; }
		void setType(EIOOBJECT_TYPE type) { _type = type; };
		void setNetwork(class CNetwork* net) { _network = net; }
		EPROTOCOL getProtocolType() const { return _protocolType; };
		void setProtocolType(EPROTOCOL type) { _protocolType = type; }
		void setKey(uint32 key) { _key = key; }
		uint32 getKey() const { return _key; }
		CAddress getLocalAddress() const { return _localAddress; }
		CAddress getRemoateAddress() const { return _remoteAddress; }
		void setLoclAddress(const CAddress& address);
		void setRemoteAddress(const CAddress& address);
		void setGroup(int32 group) { _group = group; }
		int32 getGroup() const { return _group; }

		virtual IOProtocolPtr create() const = 0;

	protected:

		virtual bool onSerialize(IOEvent* event, IBuffer* buffer) = 0;

		virtual bool onUnserialize(IIOBuffer* buffer) = 0;

		virtual void onListen(bool success) = 0;

		virtual void onUnlisten() = 0;

		virtual void onAccept(const IOProtocolPtr&) = 0;

		virtual void onClose() = 0;

		virtual void onConnect(bool success) = 0;

		virtual void onDisConnect() = 0;

		virtual bool onTimer() = 0;

	protected:
		EIOOBJECT_TYPE _type;
		EPROTOCOL _protocolType;
		uint32 _key;
		int32 _group;
		CNetwork* _network;
		CAddress _localAddress;
		CAddress _remoteAddress;
	};
}