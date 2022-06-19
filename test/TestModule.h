#pragma once
#include "Configs.h"
#include "app/Module.h"

struct SSeting
{
	uint32 code;
	uint32 type;
	std::string ip;
	uint16 port;
	std::string info;
};

class CTestModule : public app::CModule
{
public:

	bool onInit() override;

	bool _isServer{false};
	SSeting setting;
	SSeting setting2;
};