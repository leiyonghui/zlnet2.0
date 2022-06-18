#pragma once
#include "Configs.h"
#include "engine/NetEngine.h"

namespace app
{
	class CModule;
	class Server : public engine::NetEngine
	{
	public:
		Server(net::CNetwork* network);

		void sendMessage(uint32 uid, uint32 cmd, IMessagePtr message);

	protected:
		void addModule(CModule* module);

		virtual void onInit() override;
		
		virtual void onLoop() override;

		virtual void onQuit() override;

		virtual void onListen(uint32 uid, bool success) override;

		virtual void onUnlisten(uint32 uid) override;

		virtual void onNodeConnect(uint32 uid, uint32 code, uint32 type) override;

		virtual void onNodeDisConnect(uint32 uid) override;
	protected:

		std::list<Module*> _moduleList;
	};
}