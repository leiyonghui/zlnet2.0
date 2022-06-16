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
	};
	USING_SHARED_PTR(Node);

	class NetEngine : public IOEngine
	{
	public:
		NetEngine(net::CNetwork* network);

		bool setupNode(uint32 code, uint32 type, uint16 port, const ProtocolPtr& protocol);

		bool connectNode(uint32 code, uint32 type, const std::string& ip, uint16 port, const ProtocolPtr& protocol);

		void disConnectNode(uint32 code);

		uint32 getConnectNodeUid(uint32 code);

		uint32 getConnectNodeCode(uint32 uid);

	protected:
		void onInit() override;

		void handlerNodePing(CMessageContext& context);

		void handlerNodePong(CMessageContext& context);

		virtual void onTimer1000ms() override;

		virtual void onAccept(uint32 uid, uint32 fromUid) override;

		virtual void onClose(uint32 uid) override;

		virtual void onConnect(uint32 uid, bool success) override;

		virtual void onDisconnect(uint32 uid) override;

		virtual void onConnectNode(uint32 uid, uint32 code, uint32 type);

		virtual void onDisConnectNode(uint32 uid);

	private:
		uint32 _nodeUid;
		std::map<uint32, NodePtr> _connetNodes;			//<code, node>节点
		std::map<uint32, NodePtr> _connectingNodes;		//<uid, node>正在连接
		std::set<uint32> _accpetedNodes;				//<code> 等待ping
		std::map<uint32, NodePtr> _peersByUid;			//<uid, Node> 已ping/pong
	};
}