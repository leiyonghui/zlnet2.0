#pragma once
#include "Configs.h"
#include "engine/NetEngine.h"
#include "engine/Messages.h"

namespace app
{
	class CModule;
	class Server : public engine::NetEngine
	{
	public:
		Server(net::CNetwork* network);

		void sendMessage(uint32 uid, uint32 cmd, IMessagePtr message);

		void addModule(CModule* module);

	protected:
		virtual void onInit() override;
		
		virtual void onLoop() override;

		virtual void onQuit() override;

		virtual void onTimer1000ms() override;

		virtual void onAccept(uint32 uid, uint32 fromUid) override;

		virtual void onClose(uint32 uid) override;

		virtual void onConnect(uint32 uid, bool success) override;

		virtual void onDisconnect(uint32 uid) override;

		virtual void onNodeConnect(uint32 uid, uint32 code, uint32 type) override;

		virtual void onNodeDisConnect(uint32 uid) override;
	protected:

		std::list<CModule*> _moduleList;
	};
}