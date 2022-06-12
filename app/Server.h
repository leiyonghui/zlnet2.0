#pragma once
#include "Configs.h"
#include "engine/NetEngine.h"

namespace app
{
	class Server : public engine::NetEngine
	{
	public:
		Server(net::CNetwork* network);

		void sendMessage(uint32 uid, uint32 cmd, IMessagePtr message);
	};
}