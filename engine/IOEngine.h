#pragma once
#include "Configs.h"
#include "IOProtocolEx.h"

namespace network
{
	class CNetwork;
}

namespace engine
{
	class IOEngine
	{
	public:
		IOEngine();
		virtual~IOEngine();

	protected:
		class network::CNetwork* _network;
		std::unordered_map<uint32, IOProtocolPtr> _protocols;
	};
}