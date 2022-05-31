#ifdef __linux

#include <stdio.h>
//#include "network/Network.h"
#include "network/Common.h"
#include <iostream>
#include "network/Address.h"
#include "network/Network.h"
#include "engine/IOEngine.h"
#include "engine/InnerProtocol.h"
using namespace network;
using namespace std;
using namespace engine;

#ifdef __linux

#endif // __Linux

class TestNet : public IOEngine
{
public:
	TestNet(CNetwork* net) : IOEngine(net),_isServer(false){}

	bool _isServer;

	void onInit()
	{
		IOEngine::onInit();
		core_log_trace("init");
	}

	void onLoop()
	{

		auto uid = 0;
		if (_isServer)
		{
			auto protocol = std::make_shared<InnerProtocol>(EPROTO_TCP);
			uid = listen(9802, protocol);
		}
		else
		{
			auto protocol = std::make_shared<InnerProtocol>(EPROTO_TCP);
			uid = connect("127.0.0.1", 9802, protocol);
		}

		core_log_trace("onloop", uid);

		IOEngine::onLoop();


	}

	void onQuit()
	{
		core_log_trace("quit");
	}


	void onListen(uint32 uid, bool success) override
	{
		core_log_trace("onListen ", uid, success);
	}


	void onUnlisten(uint32 uid) override
	{
		core_log_trace("onUnlisten ", uid);
	}

	virtual void onAccept(uint32 uid, uint32 fromUid)
	{
		core_log_trace("accept ", uid, fromUid);
	}

	virtual void onClose(uint32 uid)
	{
		core_log_trace("close ", uid);
	}

	virtual void onConnect(uint32 uid, bool success)
	{
		core_log_trace("connect ", uid, success);
	}

	virtual void onDisconnect(uint32 uid)
	{
		core_log_trace("disconnect ", uid);
	}
};


int main(int argc, char** argv)
{
	int32 type;
	cin >> type;
	if (type == 1)
	{
		CNetwork net;
		TestNet engine(&net);
		engine._isServer = true;
		engine.run();
	}
	else
	{
		CNetwork net;
		TestNet engine(&net);
		engine.run();
	}
	//core_log_trace("test", addr.toString());
	return 0;
}

#endif // DEBUG
