#pragma once
#include "Configs.h"
#include "Protocol.h"
#include "Engine.h"

namespace engine
{
	class IOEngine : public Engine
	{
	public:
		IOEngine(net::CNetwork *network);
		virtual~IOEngine();

		ProtocolPtr getProtocol(uint32 uid);

		void run() override;

		uint32 listen(uint16 port, const ProtocolPtr& protocol);

		uint32 connect(const std::string& ip, uint16 port, const ProtocolPtr& protocol);

		void close(uint32 uid);

		void dispatchPacket(const IOPacketPtr& packet);

		void dispactchCallback(const IOPacketPtr& packet);

	protected:
		void dispatchIOPacket(PacketPtr packet);

		void onInit();

		virtual void onListen(uint32 uid, bool success);

		virtual void onUnlisten(uint32 uid);

		virtual void onAccept(uint32 uid, uint32 fromUid);

		virtual void onClose(uint32 uid);

		virtual void onConnect(uint32 uid, bool success);

		virtual void onDisconnect(uint32 uid);

	private:
		void onIOListen(PacketPtr packet);

		void onIOUnListen(PacketPtr packet);

		void onIOAccept(PacketPtr packet);

		void onIOClose(PacketPtr packet);

		void onIOConnect(PacketPtr packet);

		void onIODisconnect(PacketPtr packet);

		void onIOPacket(PacketPtr packet);

	private:
		bool removeProtocol(uint32 uid);
	protected:
		class net::CNetwork* _network;
		std::unordered_map<uint32, ProtocolPtr> _protocols;
		std::unordered_map<uint32, PacketHandlerPtr> _packetHandlers;
		std::unordered_map<uint32, CallbackHandlerPtr> _callbacks;
	};
}