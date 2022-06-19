#pragma once
#include "Configs.h"

namespace app
{
	class CModule
	{
	public:
		CModule() = default;
		virtual ~CModule() = default;

		virtual bool onInit();
		
		virtual void onQuit();

		virtual void onAccepct(uint32 uid, uint32 fromUid);

		virtual void onNodeConnect(uint32 uid, uint32 code, uint32 type);

		virtual void onNodeDisConnect(uint32 uid);
	};
}