#include "Server.h"
#include "engine/Messages.h"

namespace app
{
	Server::Server(net::CNetwork* network): engine::NetEngine(network)
	{
	}

	void Server::sendMessage(uint32 uid, uint32 cmd, IMessagePtr message)
	{
		if (std::dynamic_pointer_cast<SerializeMessage>(message))
		{
			IOPacketPtr packet = std::make_shared<IOPacket>(uid, cmd, 0, 0, message);
			send(packet);
		}
		else
		{
			SerializeMessagePtr msg = std::make_shared<SerializeMessage>(message);
			IOPacketPtr packet = std::make_shared<IOPacket>(uid, cmd, 0, 0, msg);
			send(packet);
		}
	}
}