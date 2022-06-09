#pragma once
#include "Protocol.h"

namespace engine
{
	class InnerProtocol : public Protocol
	{
		enum HeaderType
		{
			HeaderTypeCall = 0x81,
			HeaderTypeCallback = 0x82,
			HeaderTypeMessage = 0x83,
		};

#pragma pack(1)
		struct Block
		{
			uint8 type;
			uint32 size;
		};
		static_assert(sizeof(Block) == 5);

		struct HeaderCall : public Block
		{
			uint32 cmd;
			uint32 callback;
		};
		static_assert(sizeof(HeaderCall) == 13);

		struct HeaderCallback : public Block
		{
			uint32 callback;
			uint32 error;
		};
		static_assert(sizeof(HeaderCallback) == 13);

		struct HeaderMessage : public Block
		{
			uint32 cmd;
		};
		static_assert(sizeof(HeaderMessage) == 9);

	public:
		void onAwake(net::EPROTOCOL pType);

		void onRecycle();

		virtual net::IOProtocolPtr create() const override;

		virtual void onUnserialize(net::IIOBuffer* buffer) override;

		virtual void onSerialize(net::IOEvent* event, net::IBuffer* buffer) override;
	};
	USING_SHARED_PTR(InnerProtocol);
}