#pragma once
#include "core/Configs.h"
#include "core/IntrusiveNode.h"
#include "network/Configs.h"

namespace engine
{
	using namespace core;
	using net::EPROTOCOL;

	class Protocol;
	USING_SHARED_PTR(Protocol);
	class Packet;
	USING_SHARED_PTR(Packet);
	class IONotify;
	USING_SHARED_PTR(IONotify);
	class IOPacket;
	USING_SHARED_PTR(IOPacket);

	class CallbackHandler;

	class CallbackHandlerExistTag;

	class CallbackHandlerTimeoutTag;

	using CallbackHandlerExistList = IntrusiveNode<CallbackHandler, CallbackHandlerExistTag>;

	using CallbackHandlerTimeoutList = IntrusiveNode<CallbackHandler, CallbackHandlerTimeoutTag>;

	enum EPacketType
	{
		PacketListen	=	1,
		PacketUnlisten,
		PacketAccept,
		PacketConnect,
		PacketDisconnect,
		PacketClose,	
		PacketMsg,
	};
}

namespace engine
{
	enum ErrCode
	{
		ErrCode_None = 0,
		ErrCode_Default = 1,
		ErrCode_Timeout = 2,
		ErrCode_IOError = 3,
		ErrCode_SynchroTimeout = 4,
		ErrCode_IONotFound = 5,
		ErrCode_CallbackBySendError = 6,
		ErrCode_ConfirmTimeout = 7,
		//user define code from 100
	};

	struct CStartup
	{
		using Func = std::function<void()>;
		static std::map<std::string, Func>& GetList()
		{
			static std::map<std::string, Func> list;
			return list;
		}

		static void Register(const std::string& name, const Func& func)
		{
			auto& list = GetList();
			core::insert(list, name, func);
		}

		CStartup(const std::string& name, const Func& func)
		{
			Register(name, func);
		}
	};
}



#define MAKE_CODE_REGISTER(FuncName) static CStartup  __startup(#FuncName, FuncName)