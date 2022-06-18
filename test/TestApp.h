#pragma once
#include "Configs.h"
#include "app/Server.h"
#include "network/Network.h"

namespace app
{
	struct SSeting
	{
		uint32 code;
		uint32 type;
		std::string ip;
		uint16 port;
		std::string info;
	};

	class TestApp : public Server
	{
	public:
		TestApp(SSeting seting, net::CNetwork* net);
		
	};
}