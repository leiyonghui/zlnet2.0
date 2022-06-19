#pragma once
#include "Configs.h"
#include "IMessage.h"

namespace engine
{
	enum MsgCmd
	{
		Heart	= 1,
		Ping	= 2,
		Pong	= 3
	};

	struct CMessageContext
	{
		CMessageContext(uint32 uid, uint32 cmd, IMessagePtr msg):_uid(uid), _cmd(cmd), _msg(msg)
		{

		}
		uint32 _uid;
		uint32 _cmd;
		IMessagePtr _msg;
	};

	using MessageCreator = std::function<IMessagePtr()>;

	struct MessageFactory
	{
	public:
		static std::map<int32, MessageCreator>& getMessageCreators();
		static void registerMessage(int32 id, MessageCreator creator);
		static IMessagePtr Create(int32 id);
	};

	class SerializeMessage : public IMessage
	{
	public:
		SerializeMessage();

		SerializeMessage(int32 id, const net::Buffer& buffer);

		SerializeMessage(const IMessagePtr& message);

		SerializeMessage(int32 id, const char* ptr, uint32 len);

		virtual int32 identity() const override { return _id; }

		net::IBuffer* getBuffer() { return static_cast<net::IBuffer*>(&_buffer); }

		virtual void serialize(net::BufferReader& archive) override;

		virtual void serialize(net::BufferWriter& archive) const override;

	protected:
		int32 _id;
		net::Buffer _buffer;
	};
	USING_SHARED_PTR(SerializeMessage);

	//class SPingMessage : public IMessage
	//{
	//public:
	//	virtual int32 identity() const override { return ; }

	//	virtual void serialize(net::BufferReader& archive) override;

	//	virtual void serialize(net::BufferWriter& archive) const override;
	//};
	//USING_SHARED_PTR(SPingMessage);
}