#pragma once
#include "Configs.h"

namespace network
{
	struct SWritev
	{
		char* buff;
		int len;

		SWritev() :buff(nullptr), len(0) {}

		void clear() { buff = nullptr, len = 0; }
	};

}