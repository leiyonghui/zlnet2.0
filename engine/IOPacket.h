#pragma once
#include "Packet.h"
#include "IMessage.h"

namespace engine
{
	class IOPacket : public Packet
	{
	public:
		IOPacket(ProtocolPtr protocol, uint32 cmd, uint32 callbakdId, uint32  error) : Packet(PacketMsg, protocol->getKey()), _cmd(cmd), _callbackId(callbakdId), _error(error)
		{

		}

		ProtocolPtr getProtocol() const
		{
			return _protocol;
		}

		uint32 getCommand() const
		{
			return _cmd;
		}

		uint32 getCallbackId() const
		{
			return _callbackId;
		}

		uint32 getError() const
		{
			return _error;
		}

		IMessagePtr getMessage() const
		{
			return _message;
		}

	private:
		ProtocolPtr _protocol;
		uint32 _cmd;
		uint32 _callbackId;
		uint32 _error;
		IMessagePtr _message;
	};
}