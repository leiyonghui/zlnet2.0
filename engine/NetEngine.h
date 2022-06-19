#pragma once
#include "IOEngine.h"

namespace engine
{
	class Node
	{
	public:
		Node() = default;
		Node(uint32 code, uint32 type, const std::string& host, uint16 port, const ProtocolPtr& protocol) :_code(code),_type(type),_host(host),_port(port),_protocol(protocol) {
		}
		virtual ~Node() = default;

		uint32 _uid;
		uint32 _code;
		uint32 _type;
		std::string _host;
		uint16 _port;
		ProtocolPtr _protocol;
		std::string _info;
	};
	USING_SHARED_PTR(Node);

	class NetEngine : public IOEngine
	{
	public:
		NetEngine(net::CNetwork* network);

		uint32 setupNode(uint32 code, uint32 type, uint16 port, const ProtocolPtr& protocol);

		uint32 connectNode(uint32 code, uint32 type, const std::string& ip, uint16 port, const ProtocolPtr& protocol);

	protected:
		void onInit() override;

		void handlerNodePing(const IOPacketPtr& packet);

		void handlerNodePong(const IOPacketPtr& packet);

		virtual void onTimer1000ms() override;

		virtual void onUnlisten(uint32 uid) override;

		virtual void onAccept(uint32 uid, uint32 fromUid) override;

		virtual void onClose(uint32 uid) override;

		virtual void onConnect(uint32 uid, bool success) override;

		virtual void onDisconnect(uint32 uid) override;

		virtual void onNodeConnect(uint32 uid, uint32 code, uint32 type);

		virtual void onNodeDisConnect(uint32 uid);

	private:
		uint32 _nodeUid;
		uint32 _nodeCode;
		uint32 _nodeType;
		std::string _nodeInfo;
		std::map<uint32, NodePtr> _connetNodes;			//<code, node>节点
		std::map<uint32, NodePtr> _connectingNodes;		//<uid, node>正在连接
		std::set<uint32> _accpetedNodes;				//<code> 等待ping
		std::map<uint32, NodePtr> _peersByUid;			//<uid, Node> 已ping/pong
	};
}