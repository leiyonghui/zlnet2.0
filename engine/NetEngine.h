#pragma once
#include "IOEngine.h"

namespace engine
{
	class NetEngine : public IOEngine
	{
	public:
		NetEngine(network::CNetwork* network);
	};
}