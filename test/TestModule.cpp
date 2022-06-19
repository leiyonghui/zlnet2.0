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
	else
	{
		//{
		//	auto protocol = CObjectPool<InnerProtocol>::Instance()->create(net::EPROTO_TCP);
		//	auto uid = __AppInstant->setupNode(setting2.code, setting2.type, setting2.port, protocol);
		//	core_log_trace("--- setupNode", uid);
		//}
		{
			auto protocol = CObjectPool<InnerProtocol>::Instance()->create(net::EPROTO_TCP);
			auto uid = __AppInstant->connectNode(setting.code, setting.type, setting.ip, setting.port, protocol);
			__AppInstant->confirmConnect(uid, 10);
		}
	}
	return true;
}
