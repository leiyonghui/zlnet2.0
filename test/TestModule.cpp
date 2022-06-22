#include "TestModule.h"
#include "TestApp.h"
#include "engine/InnerProtocol.h"



bool CTestModule::onInit()
{
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

void CTestModule::onNodeConnect(uint32 uid, uint32 code, uint32 type)
{
	core_log_trace("node connect", uid, code, type);
}

void CTestModule::onNodeDisConnect(uint32 uid)
{
	core_log_trace("node dis connect", uid);
}