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

	void onNodeConnect(uint32 uid, uint32 code, uint32 type);

	void onNodeDisConnect(uint32 uid);

	bool _isServer{false};
	bool _isNode{ false };
	int32 _connectNum{ 0 };
	SSeting setting;
	SSeting setting2;
};