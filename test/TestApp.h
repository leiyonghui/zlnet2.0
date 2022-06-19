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
		
		virtual void onTimer1000ms() override;

		int32 _sec{0};
	};

	extern TestApp* __AppInstant;
}