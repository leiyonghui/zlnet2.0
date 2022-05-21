#pragma once
#include "Configs.h"
#include "IOProtocolEx.h"

namespace network
{
	class CNetwork;
}

namespace engine
{
	class CIOEngine
	{
	public:
		CIOEngine();
		virtual~CIOEngine();

	protected:
		class network::CNetwork* _network;
		std::unordered_map<uint32, IOProtocolPtr> _protocols;
	};
}