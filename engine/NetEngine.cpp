#include "NetEngine.h"

namespace engine
{
	NetEngine::NetEngine(net::CNetwork* network): IOEngine(network), _nodeUid(0)
	{

	}

	void NetEngine::setupNode(uint32 code, uint32 type, uint16 port, const ProtocolPtr& protocol)
	{
	}

	void NetEngine::connectNode(uint32 code, uint32 type, const std::string& ip, uint16 port, const ProtocolPtr& protocol)
	{

	}

	void NetEngine::disConnectNode(uint32 code)
	{

	}

	uint32 NetEngine::getConnectNodeUid(uint32 code)
	{
		return uint32();
	}

	uint32 NetEngine::getConnectNodeCode(uint32 uid)
	{
		return uint32();
	}

	void NetEngine::onInit()
	{
		IOEngine::onInit();

		bindPacketHandler(EMsgCmd::Ping, std::make_shared<CMessageHandlerBinding>([this](CMessageContext& context) {
			handlerNodePing(context);
		}));

		bindPacketHandler(EMsgCmd::Pong, std::make_shared<CMessageHandlerBinding>([this](CMessageContext& context) {
			handlerNodePong(context);
		}));
	}

	void NetEngine::handlerNodePing(CMessageContext& context)
	{

	}

	void NetEngine::handlerNodePong(CMessageContext& context)
	{

	}
}