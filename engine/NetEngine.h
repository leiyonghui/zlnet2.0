#pragma once
#include "IOEngine.h"

namespace engine
{
	class NetEngine : public IOEngine
	{
	public:
		NetEngine(net::CNetwork* network);
	};
}