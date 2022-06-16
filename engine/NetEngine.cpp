#include "NetEngine.h"

namespace engine
{
	NetEngine::NetEngine(net::CNetwork* network): IOEngine(network), _nodeUid(0)
	{

	}

	bool NetEngine::setupNode(uint32 code, uint32 type, uint16 port, const ProtocolPtr& protocol)
	{
		if (_nodeUid)
		{
			core_log_error("setup node exist", code, _nodeUid);
			return false;
		}
		NodePtr node = std::make_shared<Node>(code, type, "", port, protocol);
		core::insert(_connetNodes, code, node);
		_nodeUid = listen(port, protocol);
		if (!_nodeUid)
		{
			core_log_error("setup node listen error");
			return false;
		}
		if (!confirmListen(_nodeUid, 1000 * 3))
		{
			_nodeUid = 0;
			core_log_error("setup node listen error");
			return false;
		}
		core_log_trace("setup node, code:", code, "type:", type, "port:", port);
		return true;
	}

	bool NetEngine::connectNode(uint32 code, uint32 type, const std::string& ip, uint16 port, const ProtocolPtr& protocol)
	{
		if (core::exist(_connetNodes, code))
		{
			return false;
		}
		NodePtr node = std::make_shared<Node>(code, type, ip, port, protocol);
		core::insert(_connetNodes, code, node);
		node->_uid = connect(ip, port, protocol);
		if (node->_uid)
			core::insert(_connectingNodes, node->_uid, node);
		return true;
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

	void NetEngine::onTimer1000ms()
	{
		IOEngine::onTimer1000ms();

		for (auto[code, node] : _connectingNodes)
		{
			if (node->_uid) 
				continue;
			node->_uid = connect(node->_host, node->_port, std::static_pointer_cast<Protocol>(node->_protocol->create()));
			if (node->_uid) 
			{
				_connectingNodes[node->_uid] = node;
			}
		}
	}

	void NetEngine::onAccept(uint32 uid, uint32 fromUid)
	{
		IOEngine::onAccept(uid, fromUid);
	}

	void NetEngine::onClose(uint32 uid)
	{
		IOEngine::onClose(uid);
	}

	void NetEngine::onConnect(uint32 uid, bool success)
	{
		IOEngine::onConnect(uid, success);
		auto iter = _connectingNodes.find(uid);
		if (iter != _connectingNodes.end()) 
		{
			if (success)
			{
				
			}
			else
			{
				_connectingNodes.erase(iter);
			}
		}
		
	}

	void NetEngine::onDisconnect(uint32 uid)
	{
		IOEngine::onDisconnect(uid);
	}

	void NetEngine::onConnectNode(uint32 uid, uint32 code, uint32 type)
	{
	}

	void NetEngine::onDisConnectNode(uint32 uid)
	{
	}
}