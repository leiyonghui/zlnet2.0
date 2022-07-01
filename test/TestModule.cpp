#include "TestModule.h"
#include "TestApp.h"
#include "engine/InnerProtocol.h"
#include "TestMessage.h"
#include "engine/IOPacket.h"


bool CTestModule::onInit()
{

	__AppInstant->bindPacketHandler(101, std::make_shared<MessageHandlerBinding>([_isServer = _isServer](CMessageContext& context) {



		msgs::TestMessagePtr msg = std::dynamic_pointer_cast<msgs::TestMessage>(context._msg);
		if (!_isServer)
		{
			//core_log_debug("=========rev", context._uid);
			//if (msg)
				//core_log_debug("---rev", msg->value2);
			//core_log_debug("");
		}
		else
		{

		}

		IOPacketPtr packet(new IOPacket(context._uid, 101, 0, 0, std::make_shared<SerializeMessage>(msg)));
		__AppInstant->send(packet);
	}));

	if (_isServer)
	{
		auto protocol = CObjectPool<InnerProtocol>::Instance()->create(net::EPROTO_TCP);
		auto uid = __AppInstant->setupNode(setting.code, setting.type, setting.port, protocol);
		core_log_trace("--- setupNode", uid);
	}
	else if(_isNode)
	{
		auto protocol = CObjectPool<InnerProtocol>::Instance()->create(net::EPROTO_TCP);
		auto uid = __AppInstant->connectNode(setting.code, setting.type, setting.ip, setting.port, protocol);
		__AppInstant->confirmConnect(uid, 10);
	}
	else if (_connectNum)
	{
		for (int32 i = 1; i<= _connectNum; i++)
		{
			auto protocol = CObjectPool<InnerProtocol>::Instance()->create(net::EPROTO_TCP);
			__AppInstant->connect("127.0.0.1", setting.port, protocol);
		}
	}
	return true;
}

void CTestModule::onQuit()
{
	core_log_trace("module quit..");
}

void CTestModule::onAccepct(uint32 uid, uint32 fromUid)
{
	_acNum++;
	if (_acNum == 1)
	{
		_acCost = TimeHelp::clock().count();
	}
	else if(_acNum%100000 == 0)
	{
		core_log_trace("---cost", _acNum, TimeHelp::clock().count() - _acCost);
		_acCost = TimeHelp::clock().count();
	}
}

void CTestModule::onClose(uint32 uid)
{
}

void CTestModule::onConnect(uint32 uid, bool success)
{
	if (!_isServer && success)
	{
		uids.insert(uid);
	}
}

void CTestModule::onDisconnect(uint32 uid)
{
	if (!_isServer)
	{
		uids.erase(uid);
	}
}

void CTestModule::onNodeConnect(uint32 uid, uint32 code, uint32 type)
{
	core_log_trace("node connect", uid, code, type);
}

void CTestModule::onNodeDisConnect(uint32 uid)
{
	core_log_trace("node dis connect", uid);
}

void CTestModule::onTimer1000ms()
{
	for (auto uid : uids)
	{
		sendcount++;
		msgs::TestMessagePtr msg = std::make_shared<msgs::TestMessage>();
		msg->value1 = uid;
		msg->value2 = sendcount;
		msg->str = "hello world!";
		for (int32 i = 1; i <= 20; i++)
		{
			msg->values.push_back(i);
			msg->strValues.push_back(std::to_string(i));
			msg->set.insert(i);
			msg->map.insert({ i, i });
		}
		IOPacketPtr packet(new IOPacket(uid, 101, 0, 0, std::make_shared<SerializeMessage>(msg)));
		__AppInstant->send(packet);
		//core_log_trace("==sendtosever: ", uid, sendcount);
	}

	tick++;
	if (tick == 100 && _isServer)
	{
		//core_log_trace("to stop...");
		//__AppInstant->stop();
	}
}