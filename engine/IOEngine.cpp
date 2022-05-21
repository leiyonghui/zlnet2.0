#include "network/Network.h"
#include "IOEngine.h"

namespace engine
{
	IOEngine::IOEngine():_network(new network::CNetwork())
	{

	}

	IOEngine::~IOEngine()
	{
		delete _network;
	}
}