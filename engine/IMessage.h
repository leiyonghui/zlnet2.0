#pragma once
#include "network/BufferReader.h"
#include "network/BufferWriter.h"

namespace engine
{
	class IMessage
	{
	public:
		virtual ~IMessage() = default;

		//IMessage& operator = (const IMessage& o) { return *this; }

		virtual void serialize(net::BufferReader& archive) = 0;

		virtual void serialize(net::BufferWriter& archive) const = 0;

		virtual int identity() const = 0;

		//virtual IMessage* clone() const = 0;
	};
	USING_SHARED_PTR(IMessage);
}