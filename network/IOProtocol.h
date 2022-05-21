#pragma once
#include "Configs.h"
#include "IStream.h"

namespace network
{
	class IOProtocol : public CNoncopyable
	{
	public:
		IOProtocol(EPROTOCOL type, class CNetwork* net);
		virtual ~IOProtocol();

		EPROTOCOL getProtocolType() const { return _protocolType; };
		void setKey(uint32 key) { _key = key; }
		uint32 getKey() const { return _key; }
	public:
		virtual IOProtocolPtr create() const = 0;

		virtual void onSerialize(IStream* buff) = 0;

		virtual void onUnserialize() = 0;

		virtual void onListen() = 0;

		virtual void onUnlisten() = 0;

		virtual void onAccept(IOProtocolPtr&) = 0;

		virtual void onClose() = 0;

		virtual void onConnect() = 0;

		virtual void onDisconnect() = 0;
	protected:
		EPROTOCOL _protocolType;
		uint32 _key;
		CNetwork* _network;
	};
}