#pragma once
#include "Configs.h"
#include "Connection.h"

namespace network
{
	enum EEventType
	{
		ECloseCon		=	1,	//关闭连接
		EOutMessage		=	2,	//发送
		EDisConnect		=	3,	//连接关闭
 	};

	class CEventMsgBase
	{
	public:
		CEventMsgBase(EEventType type) :_type(type) {};
		virtual ~CEventMsgBase() = default;

		EEventType _type;
	};

	class CCloseEvent : public CEventMsgBase
	{
	public:
		CCloseEvent(CConnectionPtr con):CEventMsgBase(ECloseCon), _connection(con)
		{

		}

		CConnectionPtr _connection;
	};


}