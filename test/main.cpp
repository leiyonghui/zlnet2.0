#ifdef __linux

#include <stdio.h>
//#include "network/Network.h"
#include "network/Common.h"
#include <iostream>
#include "network/Address.h"
#include "engine/IOEngine.h"

using namespace network;
using namespace std;
using namespace engine;

#ifdef __linux

#endif // __Linux

class TestNet : public IOEngine
{
public:
	TestNet(CNetwork* net) : IOEngine(net){}

	void onListen(uint32 uid, bool success) override
	{
		core_log_trace("list ", uid, success);
	}


	void onUnlisten(uint32 uid) override
	{

	}

	virtual void onAccept(uint32 uid, uint32 fromUid)
	{

	}

	virtual void onClose(uint32 uid)
	{

	}

	virtual void onConnect(uint32 uid, bool success)
	{

	}

	virtual void onDisconnect(uint32 uid)
	{

	}
};


int main(int argc, char** argv)
{
	/*int32 type;
	cin >> type;
	if (type == 1)
	{
		CAddress addr("127.0.0.1", 9802);
		CNetWork network;
		network.createTcpListener(addr);
		network.start();
	}
	else
	{
		CAddress addr("127.0.0.1", 9802);
		CNetWork network;
		network.createTcpConnector(addr);
		network.start();
	}*/

	//core_log_trace("test", addr.toString());
	return 0;
}

#endif // DEBUG
