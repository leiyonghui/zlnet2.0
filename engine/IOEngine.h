#pragma once
#include "Configs.h"
#include "network/Network.h"
#include "IOProtocolEx.h"

namespace engine
{
	class CIOEngine
	{
	public:
		CIOEngine();
		virtual~CIOEngine();

	protected:
		network::CNetwork* _network;
		std::unordered_map<uint32, IOProtocolExPtr> _protocols;
	};
}