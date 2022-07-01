#include "TestApp.h"
#include "core/ResourceMonitor.h"

namespace app
{
	TestApp::TestApp(net::CNetwork* net):Server(net)
	{

	}

	void TestApp::onTimer1000ms()
	{
		Server::onTimer1000ms();

		_sec++;
		if (_sec % 30 == 0)
		{
			CObjectPoolMonitor::showInfo();
			ResourceMonitor::Instance()->showinfo();
		}

	}
	TestApp* __AppInstant = nullptr;

}
