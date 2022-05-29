#pragma once
#include "Packet.h"

namespace engine
{
	class IONotify : public Packet
	{
	public:
		IONotify(EPacketType type, const ProtocolPtr & protocol, const ProtocolPtr& fromProtocol = nullptr, bool success = false): Packet(type, protocol->getKey()),
			_protocol(protocol), _fromProtocol(fromProtocol), _success(success)
		{

		}

		ProtocolPtr getProtocol() const { return _protocol; }

		ProtocolPtr getFromProtocol() const { return _fromProtocol; }

		bool isSuccess() const { return _success; }

	private:
		ProtocolPtr _protocol;		//for listen unlisten accept close connect disconnect
		ProtocolPtr _fromProtocol;	//for accept
		bool _success;				//for listen connect
	};
}