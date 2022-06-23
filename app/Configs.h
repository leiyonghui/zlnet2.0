#pragma once
#include "engine/Configs.h"

namespace app
{
	using namespace core;
	using namespace engine;

	struct CStartup
	{
		using Func = std::function<void()>;
		static std::map<std::string, Func>& GetList()
		{
			static std::map<std::string, Func> list;
			return list;
		}

		static void Register(const std::string& name, const Func& func)
		{
			auto& list = GetList();
			core::insert(list, name, func);
		}

		CStartup(const std::string& name, const Func& func)
		{
			Register(name, func);
		}
	};

#define MAKE_CODE_REGISTER(FuncName) static CStartup  __startup(#FuncName, FuncName)

}