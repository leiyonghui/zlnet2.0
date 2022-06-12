#ifdef __linux

#include <stdio.h>
//#include "network/Network.h"
#include "network/Common.h"
#include <iostream>
#include "network/Address.h"
#include "network/Network.h"
#include "engine/IOEngine.h"
#include "engine/InnerProtocol.h"
#include "TestMessage.h"
#include "engine/IOPacket.h"
#include "core/Macro.h"
using namespace net;
using namespace std;
using namespace engine;

#ifdef __linux

#endif // __Linux

class TestNet : public IOEngine
{
public:
	TestNet(CNetwork* net) : IOEngine(net),_isServer(false),_suid(0),_cuid(0){}

	bool _isServer;
	int32 _suid;
	int32 _cuid;

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
			auto protocol = CObjectPool<InnerProtocol>::Instance()->create(net::EPROTO_TCP);
			uid = listen(9802, protocol);

			bindPacketHandler(1, std::make_shared<CMessageHandlerBinding>([](CMessageContext& context) {
				
				core_log_debug("========= msg rev");
				core_log_debug(context._cmd, context._uid);

				msgs::TestMessagePtr msg = std::dynamic_pointer_cast<msgs::TestMessage>(context._msg);
				if (msg)
				{
					msg->tostirng();
				}
			}));
		}
		else
		{
			auto protocol = CObjectPool<InnerProtocol>::Instance()->create(net::EPROTO_TCP);
			uid = connect("127.0.0.1", 9802, protocol);
		}

		core_log_trace("onloop", uid);

		IOEngine::onLoop();


	}

	void onTimer1000ms()
	{
		static int32 i = 0;
		if (++i % 30 == 1)
		{
			core::CObjectPoolMonitor::showInfo();
		}

		if (i % 30 == 29 && _suid)
		{
			msgs::TestMessagePtr msg = std::make_shared<msgs::TestMessage>();
			msg->value1 = 1024;
			msg->value2 = int64(1)<<52;
			msg->values = { 1, 2, 3, 4, 5 };
			msg->strValues = { "a", "ab", "abc" };
			msg->str = "hellow world!";
			msg->set = { 8, 7, 0 };
			msg->tostirng();
			IOPacketPtr packet(new IOPacket(_suid, 1, 0, 0, msg));
			send(packet);
			core_log_debug("---send2", _suid, msg->value1, msg->value2);
		}
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
		_cuid = uid;
	}

	virtual void onClose(uint32 uid)
	{
		core_log_trace("close ", uid);
		_cuid = 0;
	}

	virtual void onConnect(uint32 uid, bool success)
	{
		core_log_trace("connect ", uid, success);
		_suid = uid;
	}

	virtual void onDisconnect(uint32 uid)
	{
		core_log_trace("disconnect ", uid);
		_suid = 0;
		_runing = false;
	}
};


int main(int argc, char** argv)
{
	MessageFactory::registerMessage(9999, []() -> IMessagePtr {
		return std::make_shared<msgs::TestMessage>();
	});

	if (CheckCPUendian())
		std::cout << " big-endian  " << std::endl;
	else
		std::cout << " little-endian  " << std::endl;

	int32 type;
	cin >> type;
	if (type == 0)
	{
		return 0;
	}
	TimeHelp::StartUp();
	if (type == 1)
	{
		CNetwork net;
		TestNet engine(&net);
		engine._isServer = true;
		engine.run();
	}
	else if(type == 2)
	{
		CNetwork net;
		TestNet engine(&net);
		engine.run();
	}
	//core_log_trace("test", addr.toString());
	return 0;
}

#endif // DEBUG
