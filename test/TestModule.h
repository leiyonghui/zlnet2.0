#pragma once
#include "Configs.h"
#include "app/Module.h"


class CTestModule : public app::CModule
{
public:

	bool onInit() override;

	void onQuit() override;

	void onAccepct(uint32 uid, uint32 fromUid);

	void onClose(uint32 uid);

	void onConnect(uint32 uid, bool success);

	void onDisconnect(uint32 uid);

	void onNodeConnect(uint32 uid, uint32 code, uint32 type);

	void onNodeDisConnect(uint32 uid);

	void onTimer1000ms();


	bool _isServer{false};
	bool _isNode{ false };
	int32 _connectNum{ 0 };
	int32 _acNum{ 0 };
	int64 _acCost{ 0 };
	SSeting1 setting;
	SSeting1 setting2;
	int32 tick{0};
	int32 sendcount{ 0 };
	std::set<uint32> uids;
};