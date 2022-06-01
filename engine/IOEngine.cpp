#include "network/Network.h"
#include "IOEngine.h"
#include "IONotify.h"
#include "IOPacket.h"
#include <thread>

namespace engine
{
	IOEngine::IOEngine(network::CNetwork* network): Engine(), _network(network)
	{
		
	}

	IOEngine::~IOEngine()
	{
		delete _network;
	}

	ProtocolPtr IOEngine::getProtocol(uint32 uid)
	{
		auto iter = _protocols.find(uid);
		if (iter == _protocols.end()) 
		{
			return nullptr;
		}
		return iter->second;
	}

	void IOEngine::run()
	{
		std::thread ([this]() {

			core_log_trace("start net:", std::this_thread::get_id());
			_network->start();

		}).detach();

		Engine::run();
	}

	uint32 IOEngine::listen(uint16 port, const ProtocolPtr& protocol)
	{
		if (protocol->getKey())
		{
			core_log_error("listen protocol unexpected");
			return 0;
		}
		auto uid = _network->listen(port, protocol);
		if (uid <= 0)
		{
			core_log_error("listen unexpected");
			return 0;
		}
		protocol->setQueue(_msgqueue);
		if (!core::insert(_protocols, uid, protocol))
		{
			core_log_error("listen unexpected");
			return 0;
		}
		return uid;
	}

	uint32 IOEngine::connect(const std::string& ip, uint16 port, const ProtocolPtr& protocol)
	{
		if (protocol->getKey())
		{
			core_log_error("connect protocol unexpected");
			return 0;
		}
		auto uid = _network->connect(ip, port, protocol);
		if (uid <= 0)
		{
			core_log_error("connect unexpected");
			return 0;
		}
		protocol->setQueue(_msgqueue);
		if (!core::insert(_protocols, uid, protocol))
		{
			core_log_error("connect unexpected");
			return 0;
		}
		return uid;
	}

	void IOEngine::close(uint32 uid)
	{
		auto protocol = getProtocol(uid);
		if (!protocol)
		{
			core_log_error("connect unexpected", uid);
			return;
		}
		if (!protocol->isAvailable())
		{
			_network->close(uid, 2);
			removeProtocol(uid);
			return;
		}
		_network->close(uid, 2);
		switch (protocol->getType())
		{
		case network::IO_OBJECT_LISTENER:
			onUnlisten(uid);
			break;
		case network::IO_OBJECT_CONNECTION:
			onClose(uid);
			break;
		case network::IO_OBJECT_CONNECTOR:
			onDisconnect(uid);
			break;
		default:
			core_log_error("unknow close", uid);
			assert(false);
			break;
		}
		removeProtocol(uid);
	}

	void IOEngine::dispatchPacket(IOPacketPtr packet)
	{

	}

	void IOEngine::dispactchCallback(IOPacketPtr packet)
	{

	}

	void IOEngine::dispatchIOPacket(PacketPtr packet)
	{
		switch (packet->getType())
		{
		case PacketListen:
			onIOListen(packet);
			break;
		case PacketUnlisten:
			onIOUnListen(packet);
			break;
		case PacketAccept:
			onIOAccept(packet);
			break;
		case PacketConnect:
			onIOConnect(packet);
			break;
		case PacketClose:
			onIOClose(packet);
			break;
		case PacketMsg:
			onIOPacket(packet);
			break;
		case PacketDisconnect:
			onIODisconnect(packet);
			break;
		default:
			assert(false);
			break;
		}
	}

	void IOEngine::onInit()
	{
		Engine::onInit();

		bindMsgdispatcher([this](const PacketPtr& packet) {
			dispatchIOPacket(packet);
		});
	}

	void IOEngine::onListen(uint32 uid, bool success)
	{
	}

	void IOEngine::onUnlisten(uint32 uid)
	{
	}

	void IOEngine::onAccept(uint32 uid, uint32 fromUid)
	{
	}

	void IOEngine::onClose(uint32 uid)
	{
	}

	void IOEngine::onConnect(uint32 uid, bool success)
	{
	}

	void IOEngine::onDisconnect(uint32 uid)
	{
	}

	void IOEngine::onIOListen(PacketPtr packet)
	{
		IONotifyPtr notify = std::dynamic_pointer_cast<IONotify>(packet);
		auto uid = notify->getUid();
		auto protocol = getProtocol(uid);
		if (protocol != notify->getProtocol())
		{
			core_log_error("unexpect", uid);
			return;
		}
		if (notify->isSuccess())
		{
			protocol->setAvailable();
		}
		onListen(uid, notify->isSuccess());
	}

	void IOEngine::onIOUnListen(PacketPtr packet)
	{
		IONotifyPtr notify = std::dynamic_pointer_cast<IONotify>(packet);
		auto uid = notify->getUid();
		auto protocol = getProtocol(uid);
		if (protocol != notify->getProtocol())
		{
			core_log_error("unexpect", uid);
			return;
		}
		protocol->unsetAvailable();
		onListen(uid, notify->isSuccess());
		if (!removeProtocol(uid))
		{
			core_log_error("unexpect remove unlisten", uid);
		}
	}

	void IOEngine::onIOAccept(PacketPtr packet)
	{
		IONotifyPtr notify = std::dynamic_pointer_cast<IONotify>(packet);
		auto protocol = notify->getProtocol();
		auto fromPtotocol = notify->getFromProtocol();
		auto uid = protocol->getKey();
		auto fromUid = fromPtotocol->getKey();
		if (auto p = getProtocol(fromUid); p != fromPtotocol)
		{
			core_log_error("unexpect", fromPtotocol->getKey(), p ? p->getKey() : 0);
			return;
		}
		if (!fromPtotocol->isAvailable())
		{
			core_log_error("unexpect from unavailable", fromPtotocol->getKey());
			return;
		}
		protocol->setAvailable();
		if (!core::insert(_protocols, uid, protocol))
		{
			core_log_error("listen insert unexpected", protocol->getKey());
			return;
		}
		onAccept(uid, fromUid);
	}

	void IOEngine::onIOClose(PacketPtr packet)
	{
		IONotifyPtr notify = std::dynamic_pointer_cast<IONotify>(packet);
		auto protocol = notify->getProtocol();
		auto uid = protocol->getKey();
		if (!protocol->isAvailable())
		{
			core_log_warning("io close unavailable", uid);
			return;
		}
		protocol->unsetAvailable();
		onClose(uid);
		if (!removeProtocol(uid))
		{
			core_log_error("unexpect", uid);
		}
	}

	void IOEngine::onIOConnect(PacketPtr packet)
	{
		IONotifyPtr notify = std::dynamic_pointer_cast<IONotify>(packet);
		auto protocol = notify->getProtocol();
		auto uid = protocol->getKey();
		if (!core::exist(_protocols, uid))
		{
			core_log_error("unexpect connect", uid);
			return;
		}
		if (notify->isSuccess())
		{
			if (protocol->isAvailable())
			{
				core_log_error("io connect unexpect");
			}
			protocol->setAvailable();
		}
		onConnect(uid, notify->isSuccess());
	}

	void IOEngine::onIODisconnect(PacketPtr packet)
	{
		IONotifyPtr notify = std::dynamic_pointer_cast<IONotify>(packet);
		auto protocol = notify->getProtocol();
		auto uid = protocol->getKey();
		if (!core::exist(_protocols, uid))
		{
			core_log_error("io disconnect unexpect", uid);
			return;
		}
		if (!protocol->isAvailable())
		{
			core_log_warning("io disconnect unexpect unavailable", uid);
			return;
		}
		protocol->unsetAvailable();
		onDisconnect(uid);
		if (!removeProtocol(uid))
		{
			core_log_error("io disconnect unexpect", uid);
		}
	}

	void IOEngine::onIOPacket(PacketPtr packet)
	{
		IOPacketPtr ioPacket = std::dynamic_pointer_cast<IOPacket>(packet);
		auto protocol = ioPacket->getProtocol();
		if (protocol->isAvailable())
		{
			if (ioPacket->getCommand())
			{
				dispatchPacket(ioPacket);
			}
			else if (ioPacket->getCallbackId())
			{
				dispactchCallback(ioPacket);
			}
			else
			{
				core_log_error("io packet unexpect", ioPacket->getUid());
			}
		}
		else
		{
			core_log_error("io packet unexpect", ioPacket->getUid());
		}
	}

	bool IOEngine::removeProtocol(uint32 uid)
	{
		if (!core::remove(_protocols, uid))
		{
			core_log_error("remove unexpect", uid);
			return false;
		}
		return true;
	}
}