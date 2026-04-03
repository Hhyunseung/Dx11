#include "pch.h"
#include "CTimeKeeperScript.h"

CTimeKeeperScript::CTimeKeeperScript()
	: CScript(SCRIPT_TYPE::TIMEKEEPERSCRIPT)
{
}

CTimeKeeperScript::CTimeKeeperScript(CScript* _Owner)
	: CScript(SCRIPT_TYPE::TIMEKEEPERSCRIPT)
{
}

CTimeKeeperScript::~CTimeKeeperScript()
{
}


void CTimeKeeperScript::Tick()
{
}
