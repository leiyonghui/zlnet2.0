#include "Configs.h"
#include "TestApp.h"
#include "TestModule.h"
#include "network/Network.h"
#include "network/Common.h"
#include "TestMessage.h"
#include "core/ResourceMonitor.h"

using namespace std;
using namespace app;
using namespace net;

#define _MONITOR

int main(int argc, char** argv)
{

	MessageFactory::registerMessage(9999, []() -> IMessagePtr {
		return std::make_shared<msgs::TestMessage>();
	});

	if (net::common::CheckCPUendian())
		std::cout << " big-endian  " << std::endl;
	else
		std::cout << " little-endian  " << std::endl;

	SSeting1* st1 = new SSeting1(1, 1, "", 9802, "server1");
	SSeting1* st2 = new SSeting1(2, 2, "127.0.0.1", 9803, "server2");
	std::string str = "101.33.228.226";
	std::cout << st1->ip.empty() << "  " << st2->ip.empty() <<"  "<<str.empty()<< "  "<<st1->info<<std::endl;
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
	FileLogAppender * apperder = new FileLogAppender(string("Test_Net_").append(op == 1 ? "Server" : "Client").append("_%T"), true);
	Logger::Instance(apperder);
	ResourceMonitor::Instance(new ResourceMonitor());

	CNetwork net;
	TestApp app(&net);
	__AppInstant = &app;
	CTestModule module;
	module.setting = *st1;
	module.setting2 = *st2;
	app.addModule(&module);
	if (op == 1)
	{
		module._isServer = true;
	}
	else if (op == 2)
	{
		module._isNode = true;
	}
	else if (op == 3)
	{
		if (argc == 3)
		{
			module._connectNum = std::atoi(argv[2]);
		}
		else
		{
			cin >> module._connectNum;
		}
	}
	else
	{
		return 0;
	}
	net.start();
	app.run();
	net.stop();
	core_log_trace("...main end!!!");
	Logger::Instance()->stop();
	delete apperder;
	return 0;
}