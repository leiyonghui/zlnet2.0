#pragma once
#include "Configs.h"
#include "app/Module.h"


class CTestModule : public app::CModule
{
public:

	bool onInit() override;

	void onAccepct(uint32 uid, uint32 fromUid);

	void onNodeConnect(uint32 uid, uint32 code, uint32 type);

	void onNodeDisConnect(uint32 uid);

	bool _isServer{false};
	bool _isNode{ false };
	int32 _connectNum{ 0 };
	int32 _acNum{ 0 };
	int64 _acCost{ 0 };
	SSeting1 setting;
	SSeting1 setting2;
};