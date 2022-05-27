#pragma once
#include "Configs.h"
#include "IStream.h"
#include "IOEvent.h"
#include "IBuffer.h"

namespace network
{
	class IOProtocol : public CNoncopyable, public std::enable_shared_from_this<IOProtocol>
	{
		friend class CNetwork;
	public:
		IOProtocol(EPROTOCOL type);
		virtual ~IOProtocol();

		void setNetwork(class CNetwork* net) { _network = net; }
		EPROTOCOL getProtocolType() const { return _protocolType; };
		void setKey(uint32 key) { _key = key; }
		uint32 getKey() const { return _key; }
	protected:
		virtual IOProtocolPtr create() const = 0;

		virtual void onSerialize(IStream* buffer) = 0;

		virtual void onUnserialize(IOEvent* event, IBuffer* buffer) = 0;

		virtual void onListen(bool success) = 0;

		virtual void onUnlisten() = 0;

		virtual void onAccept(IOProtocolPtr&) = 0;

		virtual void onClose() = 0;

		virtual void onConnect(bool dis) = 0;

		virtual void onDisConnect() = 0;
	protected:
		EPROTOCOL _protocolType;
		uint32 _key;
		CNetwork* _network;
	};
}