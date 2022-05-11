#include "IOEngine.h"


namespace engine
{
	CIOEngine::CIOEngine():_network(new network::CNetwork())
	{

	}

	CIOEngine::~CIOEngine()
	{
		delete _network;
	}
}