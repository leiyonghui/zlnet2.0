#include "Server.h"
#include "Module.h"
#include "engine/Messages.h"
#include "engine/IOPacket.h"

namespace app
{
	Server::Server(net::CNetwork* network): engine::NetEngine(network)
	{
	}

	void Server::sendMessage(uint32 uid, uint32 cmd, IMessagePtr message)
	{
		if (std::dynamic_pointer_cast<SerializeMessage>(message))
		{
			IOPacketPtr packet = std::make_shared<IOPacket>(uid, cmd, 0, 0, message);
			send(packet);
		}
		else
		{
			SerializeMessagePtr msg = std::make_shared<SerializeMessage>(message);
			IOPacketPtr packet = std::make_shared<IOPacket>(uid, cmd, 0, 0, msg);
			send(packet);
		}
	}

	void Server::addModule(CModule* module)
	{
		_moduleList.push_back(module);
	}

	void Server::onInit()
	{
		NetEngine::onInit();
	}

	void Server::onLoop()
	{
		for (auto module : _moduleList)
		{
			if (!module->onInit())
				return;
		}

		NetEngine::onLoop();
		
		for (auto module : _moduleList)
		{
			module->onQuit();
		}
	}

	void Server::onQuit()
	{
		NetEngine::onQuit();
	}

	void Server::onTimer1000ms()
	{
		NetEngine::onTimer1000ms();

		for (auto module : _moduleList)
		{
			module->onTimer1000ms();
		}
	}

	void Server::onAccept(uint32 uid, uint32 fromUid)
	{
		NetEngine::onAccept(uid, fromUid);

		for (auto module : _moduleList)
		{
			module->onAccepct(uid, fromUid);
		}
	}

	void Server::onClose(uint32 uid)
	{
		NetEngine::onClose(uid);

		for (auto module : _moduleList)
		{
			module->onClose(uid);
		}
	}

	void Server::onConnect(uint32 uid, bool success)
	{
		NetEngine::onConnect(uid, success);

		for (auto module : _moduleList)
		{
			module->onConnect(uid, success);
		}
	}

	void Server::onDisconnect(uint32 uid)
	{
		NetEngine::onDisconnect(uid);

		for (auto module : _moduleList)
		{
			module->onDisconnect(uid);
		}
	}

	void Server::onNodeConnect(uint32 uid, uint32 code, uint32 type)
	{
		NetEngine::onNodeConnect(uid, code, type);

		for (auto module : _moduleList)
		{
			module->onNodeConnect(uid, code, type);
		}
	}

	void Server::onNodeDisConnect(uint32 uid)
	{
		NetEngine::onNodeDisConnect(uid);

		for (auto module : _moduleList)
		{
			module->onNodeDisConnect(uid);
		}
	}
}