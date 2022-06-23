#include "NetEngine.h"
#include "Serializes.h"
#include "IOPacket.h"
#include "Messages.h"

namespace engine
{
	NetEngine::NetEngine(net::CNetwork* network): IOEngine(network), _nodeUid(0), _nodeCode(0), _nodeType(0)
	{

	}

	uint32 NetEngine::setupNode(uint32 code, uint32 type, uint16 port, const ProtocolPtr& protocol, std::string info)
	{
		if (_nodeUid)
		{
			core_log_error("setup node exist", code, _nodeUid);
			return 0;
		}
		NodePtr node = std::make_shared<Node>(code, type, "", port, protocol);
		_nodeUid = listen(port, protocol);
		_nodeCode = code;
		_nodeType = type;
		_nodeInfo = info;
		if (!_nodeUid)
		{
			core_log_error("setup node listen error");
			return 0;
		}
		if (!confirmListen(_nodeUid, 1000 * 3))
		{
			_nodeUid = 0;
			core_log_error("setup node listen error");
			return 0;
		}
		node->_uid = _nodeUid;
		core_log_trace("setup node, code:", code, "type:", type, "port:", port);
		return _nodeUid;
	}

	uint32 NetEngine::connectNode(uint32 code, uint32 type, const std::string& ip, uint16 port, const ProtocolPtr& protocol)
	{
		if (core::exist(_connetNodes, code))
		{
			return 0;
		}
		NodePtr node = std::make_shared<Node>(code, type, ip, port, protocol);
		core::insert(_connetNodes, code, node);
		node->_uid = connect(ip, port, protocol);
		if (node->_uid)
			core::insert(_connectingNodes, node->_uid, node);
		return node->_uid;
	}

	void NetEngine::onInit()
	{
		IOEngine::onInit();

		bindPacketHandler(MsgCmd::Ping, std::make_shared<PacketHandlerImpl>([this](const IOPacketPtr& packet) {
			handlerNodePing(packet);
		}));

		bindPacketHandler(MsgCmd::Pong, std::make_shared<PacketHandlerImpl>([this](const IOPacketPtr& packet) {
			handlerNodePong(packet);
		}));
	}

	void NetEngine::handlerNodePing(const IOPacketPtr& packet)
	{
		SerializeMessagePtr msg = std::dynamic_pointer_cast<SerializeMessage>(packet->getMessage());
		if (!msg) 
		{
			core_log_error("handler ping msg null", packet->getUid());
			close(packet->getUid());
			return;
		}
		uint32 fromCode = 0;
		uint32 fromType = 0; 
		uint32 toCode = 0;
		uint32 toType = 0;
		std::string info;
		net::BufferReader reader(msg->getBuffer());
		engine::unpack(reader, fromCode, fromType, info, toCode, toType);
		if (toCode != _nodeCode || toType != _nodeType) 
		{
			core_log_error("connect node error", fromCode, fromType, toCode, toType, info);
			close(packet->getUid());
			return;
		}
		if (!core::exist(_accpetedNodes, packet->getUid())) 
		{
			core_log_error("unexpect accpeted node", packet->getUid());
			close(packet->getUid());
			return;
		}
		NodePtr node = std::make_shared<Node>();
		node->_uid = packet->getUid();
		node->_code = fromCode;
		node->_type = fromType;
		node->_info = info;
		if (!core::insert(_peersByUid, packet->getUid(), node)) 
		{
			core_log_error("unexpect peer node", packet->getUid(), fromCode);
			close(packet->getUid());
			return;
		}
		node->_info = info;

		SerializeMessagePtr pong = std::make_shared<SerializeMessage>();
		net::BufferWriter writer(pong->getBuffer());
		engine::pack(writer, _nodeCode, _nodeType, _nodeInfo, node->_code, node->_type);
		IOPacketPtr pongPacket = std::make_shared<IOPacket>(node->_uid, MsgCmd::Pong, 0, 0, pong);
		send(pongPacket);

		onNodeConnect(packet->getUid(), fromCode, fromType);
	}

	void NetEngine::handlerNodePong(const IOPacketPtr& packet)
	{
		SerializeMessagePtr msg = std::dynamic_pointer_cast<SerializeMessage>(packet->getMessage());
		if (!msg)
		{
			core_log_error("handler ping msg null", packet->getUid());
			close(packet->getUid());
			return;
		}
		uint32 fromCode = 0;
		uint32 fromType = 0;
		uint32 toCode = 0;
		uint32 toType = 0;
		std::string info;
		net::BufferReader reader(msg->getBuffer());
		engine::unpack(reader, fromCode, fromType, info, toCode, toType);
		if (toCode != _nodeCode || toType != _nodeType)
		{
			core_log_error("pong node error", packet->getUid(), fromCode, fromType, toCode, toType, info);
			close(packet->getUid());
			return;
		}
		auto node = core::find(_connectingNodes, packet->getUid(), NodePtr());
		if (!node) 
		{
			core_log_error("pong node error", packet->getUid(), fromCode, fromType, toCode, toType, info);
			close(packet->getUid());
			return;
		}
		core::remove(_accpetedNodes, packet->getUid());
		if (node->_code != fromCode || node->_type != fromType) 
		{
			core_log_error("pong node error", packet->getUid(), fromCode, fromType, toCode, toType, node->_code, node->_type, info);
			close(packet->getUid());
			return;
		}
		if (!core::insert(_peersByUid, packet->getUid(), node))
		{
			core_log_error("pong node error", packet->getUid(), fromCode, fromType, toCode, toType, info);
			close(packet->getUid());
			return;
		}
		node->_info = info;
		onNodeConnect(packet->getUid(), node->_code, node->_type);
	}

	void NetEngine::onTimer1000ms()
	{
		IOEngine::onTimer1000ms();

		for (auto[code, node] : _connetNodes)
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

	void NetEngine::onUnlisten(uint32 uid)
	{
		if (uid == _nodeUid) 
		{
			_nodeUid = 0;
			_nodeCode = 0;
			_nodeType = 0;
			_nodeInfo = "";
		}
	}

	void NetEngine::onAccept(uint32 uid, uint32 fromUid)
	{
		IOEngine::onAccept(uid, fromUid);

		if (_nodeUid == fromUid)
		{
			if (!core::insert(_accpetedNodes, uid))
			{
				core_log_error("accept node existed", uid);
				close(uid);
			}			
		}
	}

	void NetEngine::onClose(uint32 uid)
	{
		IOEngine::onClose(uid);

		if (core::exist(_accpetedNodes, uid)) 
		{
			core::remove(_accpetedNodes, uid);
			core_log_trace("remove accpected node", uid);
		}

		auto iter = _peersByUid.find(uid);
		if (iter != _peersByUid.end())
		{
			core_log_trace("remove peer node", uid, iter->second->_code);
			onNodeDisConnect(uid);
			iter->second->_uid = 0;
			_peersByUid.erase(iter);
		}
	}

	void NetEngine::onConnect(uint32 uid, bool success)
	{
		IOEngine::onConnect(uid, success);
		auto iter = _connectingNodes.find(uid);
		if (iter != _connectingNodes.end()) 
		{
			auto& node = iter->second;
			if (success)
			{
				SerializeMessagePtr msg = std::make_shared<SerializeMessage>();
				net::BufferWriter writer(msg->getBuffer());
				engine::pack(writer, _nodeCode, _nodeType, _nodeInfo, node->_code, node->_type);
				IOPacketPtr packet = std::make_shared<IOPacket>(node->_uid, MsgCmd::Ping, 0, 0, msg);
				send(packet);
			}
		}
		
	}

	void NetEngine::onDisconnect(uint32 uid)
	{
		IOEngine::onDisconnect(uid);

		if (auto node = core::find(_connectingNodes, uid, NodePtr()))
		{
			core::remove(_connectingNodes, uid);
			node->_uid = 0;
		}

		if (auto node = core::find(_peersByUid, uid, NodePtr()))
		{
			onNodeDisConnect(uid);
			node->_uid = 0;
			_peersByUid.erase(uid);
		}
	}

	void NetEngine::onNodeConnect(uint32 uid, uint32 code, uint32 type)
	{

	}

	void NetEngine::onNodeDisConnect(uint32 uid)
	{

	}
}