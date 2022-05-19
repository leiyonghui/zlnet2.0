#pragma once
#include "Configs.h"

namespace network
{
	class IOProtocol : public CNoncopyable
	{
	public:
		IOProtocol(EPROTOCOL type, class CNetwork* net);

		virtual ~IOProtocol();

		EPROTOCOL getProto() const { return _protocolType; };
		void setKey(uint32 key) { _key = key; }
		uint32 key() const { return _key; }
	public:
		virtual IOProtocol* create() const = 0;

		virtual void onSerialize() = 0;

		virtual void onUnserialize() = 0;

		virtual void onListen() = 0;

		virtual void onUnlisten() = 0;

		virtual void onAccept() = 0;

		virtual void onClose() = 0;

		virtual void onConnect() = 0;

		virtual void onDisconnect() = 0;
	protected:
		EPROTOCOL _protocolType;
		uint32 _key;
		CNetwork* _network;
	};
	USING_SHARED_PTR(IOProtocol);
}