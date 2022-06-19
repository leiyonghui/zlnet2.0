#include "Module.h"

namespace app
{
	bool CModule::onInit()
	{
		return true;
	}
	void CModule::onQuit()
	{
	}
	void CModule::onAccepct(uint32 uid, uint32 fromUid)
	{

	}
	void CModule::onNodeConnect(uint32 uid, uint32 code, uint32 type)
	{
	}
	void CModule::onNodeDisConnect(uint32 uid)
	{
	}
}