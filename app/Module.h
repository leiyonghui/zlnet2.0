#pragma once
#include "Configs.h"

namespace app
{
	class CModule
	{
	public:
		CModule() = default;
		virtual ~CModule() = default;

		virtual bool onInit() = 0;
		
		virtual void onQuit() = 0;

		virtual void onNodeConnect(uint32 uid, uint32 code, uint32 type) = 0;

		virtual void onNodeDisConnect(uint32 uid) = 0;
	};
}