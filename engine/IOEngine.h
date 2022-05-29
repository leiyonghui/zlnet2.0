#pragma once
#include "Configs.h"
#include "Protocol.h"
#include "Engine.h"

namespace engine
{
	class IOEngine : public Engine
	{
	public:
		IOEngine(network::CNetwork *network);
		virtual~IOEngine();

		ProtocolPtr getProtocol(uint32 uid);

		uint32 listen(uint16 port, const ProtocolPtr& protocol);

		uint32 connect(const std::string& ip, uint16 port, const ProtocolPtr& protocol);

		void close(uint32 uid);

		void dispatchPacket(class IOPacket* packet);

		void dispactchCallback(IOPacket* packet);
	protected:
		void dispatchIOPacket(Packet* packet);

		virtual void onListen(uint32 uid, bool success);

		virtual void onUnlisten(uint32 uid);

		virtual void onAccept(uint32 uid, uint32 fromUid);

		virtual void onClose(uint32 uid);

		virtual void onConnect(uint32 uid, bool success);

		virtual void onDisconnect(uint32 uid);

	private:
		void onIOListen(Packet* packet);

		void onIOUnListen(Packet* packet);

		void onIOAccept(Packet* packet);

		void onIOClose(Packet* packet);

		void onIOConnect(Packet* packet);

		void onIODisconnect(Packet* packet);

		void onIOPacket(Packet* packet);

	private:
		bool removeObject(uint32 uid);
	protected:
		class network::CNetwork* _network;
		std::unordered_map<uint32, ProtocolPtr> _protocols;
	};
}