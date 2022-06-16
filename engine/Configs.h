#pragma once
#include "core/Configs.h"
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
	//class PacketHandler;
	//USING_SHARED_PTR(PacketHandler);
	//class CallbackHandler;
	//USING_SHARED_PTR(CallbackHandler);

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
}