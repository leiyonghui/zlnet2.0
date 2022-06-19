#pragma once
#include "app/Configs.h"

using namespace app;

namespace msgs
{
	using namespace core;
	using namespace engine;
}

namespace app
{
	class SSeting1
	{
	public:
		SSeting1() = default;
		SSeting1(uint32 c, uint32 t, const std::string& i, uint16 p, const std::string& f) : code(c), type(t), ip(i), port(p), info(f) {
			std::cout << "ssss2" << std::endl;
		}
		uint32 code;
		uint32 type;
		std::string ip;
		uint16 port;
		std::string info;
	};


}