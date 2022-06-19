#include "Configs.h"
#include "TestApp.h"
#include "TestModule.h"
#include "network/Network.h"

using namespace std;
using namespace app;
using namespace net;


int main(int argc, char** argv)
{
	if (CheckCPUendian())
		std::cout << " big-endian  " << std::endl;
	else
		std::cout << " little-endian  " << std::endl;

	SSeting st1 = {
		1,1,"",9802,"server1"
	};
	SSeting st2 = {
		2,2,"",9803,"server2"
	};
	int32 op = 0;
	if (argc == 1)
	{
		cin >> op;
	}
	if (argc == 2) 
	{
		op = std::atoi(argv[1]);
	}
	TimeHelp::StartUp();
	if (op == 1)
	{
		CNetwork net;
		TestApp app1(&net);
		__AppInstant = &app1;
		CTestModule module;
		module.setting = st1;
		module.setting2 = st2;
		module._isServer = true;
		app1.addModule(&module);
		net.start();
		app1.run();
		net.stop();
	}
	else if (op == 2)
	{
		CNetwork net;
		TestApp app1(&net);
		__AppInstant = &app1;
		CTestModule module;
		module.setting = st1;
		module.setting2 = st2;
		app1.addModule(&module);
		net.start();
		app1.run();
		net.stop();
	}
	return 0;
}