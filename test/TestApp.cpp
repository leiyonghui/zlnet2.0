#include "TestApp.h"

namespace app
{
	TestApp::TestApp(net::CNetwork* net):Server(net)
	{

	}
	TestApp* __AppInstant = nullptr;
}
