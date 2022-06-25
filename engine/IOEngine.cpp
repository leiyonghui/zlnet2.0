#include "PacketHandlers.h"
#include "network/Network.h"
#include "IOEngine.h"
#include "IONotify.h"
#include "IOPacket.h"
#include "Messages.h"

namespace engine
{
	const int32 MAX_HEART_COUNT = 60;

	IOEngine::IOEngine(net::CNetwork* network): Engine(), _network(network), _nextCallbackId(0), _callbackTimeoutMs(10000)
	{
		
	}

	IOEngine::~IOEngine()
	{
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
		protocol->setQueue(getMsgQueue());
		if (!core::insert(_protocols, uid, protocol))
		{
			core_log_error("listen unexpected");
			return 0;
		}
		return uid;
	}

	bool IOEngine::confirmListen(uint32 uid, int64 timeout)
	{
		auto protocol = getProtocol(uid);
		if (!protocol)
		{
			return false;
		}
		if (protocol->isAvailable())
		{
			return true;
		}
		pollOnePacket(timeout, [protocol](const PacketPtr& packet) -> bool {
			if (IONotifyPtr notity = std::dynamic_pointer_cast<IONotify>(packet))
			{
				if (notity->getProtocol() == protocol)
				{
					return true;
				}
			}
			return false;
		});
		if (protocol->isAvailable())
		{
			return true;
		}
		removeProtocol(uid);		 //confircom²»ÓÃ´¥·¢unlisten
		_network->close(uid, 0);   
		core_log_error("listen confircom error", uid);
		return false;
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
		protocol->setQueue(getMsgQueue());
		if (!core::insert(_protocols, uid, protocol))
		{
			core_log_error("connect unexpected");
			return 0;
		}
		return uid;
	}

	bool IOEngine::confirmConnect(uint32 uid, int64 timeout)
	{
		auto protocol = getProtocol(uid);
		if (!protocol)
		{
			return false;
		}
		if (protocol->isAvailable())
		{
			return true;
		}
		pollOnePacket(timeout, [protocol](const PacketPtr& packet)->bool {
			if (IONotifyPtr notity = std::dynamic_pointer_cast<IONotify>(packet))
			{
				if (notity->getProtocol() == protocol)
				{
					return true;
				}
			}
			return false;
		});
		if (protocol->isAvailable())
		{
			return true;
		}
		removeProtocol(uid);
		_network->close(uid, 0);
		return false;
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
		case net::IO_OBJECT_LISTENER:
			onUnlisten(uid);
			break;
		case net::IO_OBJECT_CONNECTION:
			onClose(uid);
			break;
		case net::IO_OBJECT_CONNECTOR:
			onDisconnect(uid);
			break;
		default:
			core_log_error("unknow close", uid);
			assert(false);
			break;
		}
		removeProtocol(uid);
	}

	void IOEngine::send(const IOPacketPtr& packet)
	{
		send(CallbackHandlerPtr(), packet);
	}

	void IOEngine::send(const std::function<void(const IOPacketPtr&)>& func, const IOPacketPtr& packet)
	{
		send(std::make_shared<CallbackHandlerImpl>(func), packet);
	}

	void IOEngine::send(const CallbackHandlerPtr& callbackHander, const IOPacketPtr& packet)
	{
		auto protocol = getProtocol(packet->getUid());
		if (packet->getCommand())
		{
			if (protocol && protocol->isAvailable())
			{
				if (callbackHander)
				{
					auto cbId = bindCallbackHandler(callbackHander);
					packet->setCallbackId(cbId);
					callbackHander->_uid = packet->getUid();
					callbackHander->_cmd = packet->getCommand();
					protocol->getCallbackList().pushBack(callbackHander.get());
				}
				_network->send<IOPacketPtr>(new net::IOEventData<IOPacketPtr>(packet->getUid(), packet));
			}
			else
			{
				assert(packet->getUid() == 0);
				if (callbackHander)
				{
					uint32 cbId = bindCallbackHandler(callbackHander);
					callbackHander->_uid = packet->getUid();
					callbackHander->_cmd = packet->getCommand();
					postPacket(std::make_shared<IOPacket>(0, 0, cbId, ErrCode_CallbackBySendError));
				}
			}
		}
		else if(packet->getCallbackId())
		{
			assert(!callbackHander);
			if (protocol && protocol->isAvailable())
			{
				_network->send<IOPacketPtr>(new net::IOEventData<IOPacketPtr>(packet->getUid(), packet));
			}
			else
			{
				core_log_warning("send protocol unavilable", packet->getUid());
			}
		}
		else
		{
			core_log_error("unexpect send", packet->getUid());
			assert(false);
		}
	}

	void IOEngine::dispatchPacket(const IOPacketPtr& packet)
	{
		auto iter = _packetHandlers.find(packet->getCommand());
		if (iter == _packetHandlers.end())
		{
			core_log_error("cmd no handler", packet->getCommand());
		}
		else
		{
			iter->second->onPacket(packet);
		}
	}

	void IOEngine::dispactchCallback(const IOPacketPtr& packet)
	{
		auto cbId = packet->getCallbackId();
		auto iter = _callbackHandlers.find(cbId);
		if (iter == _callbackHandlers.end())
		{
			core_log_warning("no callback handler", packet->getUid(), packet->getError());
			return;
		}
		assert(iter->second->_uid == packet->getUid());
		auto handler = iter->second;
		core::remove(_callbackHandlers, cbId);
		handler->CallbackHandlerExistList::leave();
		handler->CallbackHandlerTimeoutList::leave();
		handler->onPacket(packet);
	}

	bool IOEngine::bindPacketHandler(uint32 cmd, const PacketHandlerPtr& handler)
	{
		return _packetHandlers.insert({cmd, handler}).second;
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

		bindPacketHandler(MsgCmd::Heart, std::make_shared<MessageHandlerBinding>([this](CMessageContext& context) {
			handlerProtocolHeart(context);
		}));
	}

	void IOEngine::onQuit()
	{
		Engine::onQuit();

	}

	void IOEngine::onTimer1000ms()
	{
		Engine::onTimer1000ms();
		
		onTimerCallbackTimeout();

		onTimerProtocolHeart();
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
			core_log_warning("unexpect io listen", uid);
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
		if (!protocol)
		{
			core_log_warning("unexpect io on unlisten", uid);
			return;
		}
		if (!protocol->isAvailable())
		{
			core_log_error("unexpect available", uid);
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
			core_log_error("unexpect close", uid);
		}
	}

	void IOEngine::onIOConnect(PacketPtr packet)
	{
		IONotifyPtr notify = std::dynamic_pointer_cast<IONotify>(packet);
		auto protocol = notify->getProtocol();
		auto uid = protocol->getKey();
		if (!core::exist(_protocols, uid))
		{
			core_log_warning("unexpect connect no protocol", uid);
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
		auto protocol = getProtocol(packet->getUid());
		if (protocol && protocol->isAvailable())
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
		core_log_trace("remove protocol", uid, _protocols.size());
		return true;
	}

	uint32 IOEngine::makeCallbackId()
	{
		if (_nextCallbackId < 0xffffffff)
			_nextCallbackId++;
		else 
			_nextCallbackId = 1;
		if (!_callbackTimeOutList.empty() && _callbackTimeOutList.front()->_cbId == _nextCallbackId)
			return 0;
		return _nextCallbackId;
	}

	uint32 IOEngine::bindCallbackHandler(const CallbackHandlerPtr& handler)
	{
		assert(handler->_cbId == 0);
		auto cbId = makeCallbackId();
		assert(cbId);
		handler->_cbId = cbId;
		handler->_toMs = TimeHelp::clock().count() + _callbackTimeoutMs;
		_callbackHandlers[cbId] = handler;
		_callbackTimeOutList.pushBack(handler.get());
		return cbId;
	}

	void IOEngine::onTimerCallbackTimeout()
	{
		auto now = TimeHelp::clock().count();
		while (!_callbackTimeOutList.empty())
		{
			auto handler = _callbackTimeOutList.front();
			if (now < handler->_toMs)
			{
				break;
			}
			core_log_error("callback time out", handler->_uid, handler->_toMs);
			core::remove(_callbackHandlers, handler->_cbId);
			handler->CallbackHandlerExistList::leave();
			handler->CallbackHandlerTimeoutList::leave();
			IOPacketPtr packet = std::make_shared<IOPacket>(handler->_uid, handler->_cmd,handler->_cbId, ErrCode_Timeout);
			handler->onPacket(packet);
		}
	}

	void IOEngine::onTimerProtocolHeart()
	{
		std::vector<uint32> removes;
		for (auto&[uid, protocol] : _protocols)
		{
			if (!protocol->isAvailable())
				continue;
			if (protocol->getType() == net::IO_OBJECT_LISTENER)
				continue;
			if (protocol->getType() == net::IO_OBJECT_CONNECTOR)
			{
				IOPacketPtr packet(new IOPacket(uid, MsgCmd::Heart, 0, 0, nullptr));
				send(packet);
			}
			else if (protocol->getType() == net::IO_OBJECT_CONNECTION)
			{
				auto count = protocol->getHeartCount() + 1;
				if (count > MAX_HEART_COUNT) {
					core_log_trace("protocol heart close", uid, count);
					removes.push_back(uid);
				}
				else
					protocol->setHeartCount(count);
			}
		}
		for (auto uid : removes)
		{
			close(uid);
		}
	}

	void IOEngine::handlerProtocolHeart(CMessageContext& context)
	{
		auto protocol = getProtocol(context._uid);
		if (protocol && protocol->isAvailable())
		{
			protocol->setHeartCount(0);
		}
		else 
		{
			core_log_error("unknow heart", context._uid);
		}
	}
}