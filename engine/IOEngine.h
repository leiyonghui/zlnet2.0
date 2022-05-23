#pragma once
#include "Configs.h"
#include "IOProtocolEx.h"
#include "Engine.h"
#include "core/MsgQueue.h"

namespace engine
{
	class IOEngine : public Engine
	{
	public:
		IOEngine();
		virtual~IOEngine();

	protected:
		class network::CNetwork* _network;
		std::unordered_map<uint32, IOProtocolPtr> _protocols;
	};
}