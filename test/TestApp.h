#pragma once
#include "Configs.h"
#include "app/Server.h"
#include "network/Network.h"

namespace app
{
	class TestApp : public Server
	{
	public:
		TestApp(net::CNetwork* net);
		
	};

	extern TestApp* __AppInstant;
}