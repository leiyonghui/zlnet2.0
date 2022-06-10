#pragma once
#include "Packet.h"
#include "IMessage.h"
#include "Protocol.h"

namespace engine
{
	class IOPacket : public Packet
	{
	public:
		IOPacket(int32 uid, uint32 cmd, uint32 callbakdId, uint32  error) : Packet(PacketMsg, uid), _cmd(cmd), _callbackId(callbakdId), _error(error)
		{

		}

		IOPacket(int32 uid, uint32 cmd, uint32 callbakdId, uint32  error, const IMessagePtr& message) : Packet(PacketMsg, uid), _cmd(cmd), _callbackId(callbakdId), _error(error), _message(message)
		{

		}

		uint32 getCommand() const
		{
			return _cmd;
		}

		uint32 getCallbackId() const
		{
			return _callbackId;
		}

		void setCallbackId(uint32 cdId)
		{
			_callbackId = cdId;
		}

		uint32 getError() const
		{
			return _error;
		}

		IMessagePtr getMessage() const
		{
			return _message;
		}

		void setMessage(const IMessagePtr& message)
		{
			_message = message;
		}

	private:
		uint32 _cmd;
		uint32 _callbackId;
		uint32 _error;
		IMessagePtr _message;
	};
}