#include "Configs.h"
#include "TestApp.h"
#include "TestModule.h"
#include "network/Network.h"
#include "network/Common.h"

using namespace std;
using namespace app;
using namespace net;


int main(int argc, char** argv)
{
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

	CNetwork net;
	TestApp app1(&net);
	__AppInstant = &app1;
	CTestModule module;
	module.setting = *st1;
	module.setting2 = *st2;
	app1.addModule(&module);
	if (op == 1)
	{
		module._isServer = true;
		net.start();
		app1.run();
		net.stop();
	}
	else if (op == 2)
	{
		module._isNode = true;
		net.start();
		app1.run();
		net.stop();
	}
	else if (op == 3)
	{
		cin >> module._connectNum;
		net.start();
		app1.run();
		net.stop();
	}
	else if (op == 4)
	{
		core_log_trace("--------1");
		core_log_trace("--------2");
		core_log_trace("--------3");
		core_log_trace("--------4");
	}
	Logger::Instance()->stop();
	delete apperder;
	return 0;
}