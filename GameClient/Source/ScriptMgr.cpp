#include "pch.h"
#include "ScriptMgr.h"

#include "Scripts/CCamMoveScript.h"
#include "Scripts/CEnemy.h"
#include "Scripts/CLycheeScript.h"
#include "Scripts/CMissileScript.h"
#include "Scripts/CMonsterScript.h"
#include "Scripts/CPlayerScript.h"
#include "Scripts/CJumpState.h"
#include "Scripts/CRunState.h"
#include "Scripts/StateMachine.h"

void ScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCamMoveScript");
	_vec.push_back(L"CEnemy");
	_vec.push_back(L"CLycheeScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"JumpState");
	_vec.push_back(L"RunState");
	_vec.push_back(L"StateMachine");
}

CScript * ScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCamMoveScript" == _strScriptName)
		return new CCamMoveScript;
	if (L"CEnemy" == _strScriptName)
		return new CEnemy;
	if (L"CLycheeScript" == _strScriptName)
		return new CLycheeScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"JumpState" == _strScriptName)
		return new CJumpState;
	if (L"RunState" == _strScriptName)
		return new CRunState;
	if (L"StateMachine" == _strScriptName)
		return new StateMachine;
	return nullptr;
}

CScript * ScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMMOVESCRIPT:
		return new CCamMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::ENEMY:
		return new CEnemy;
		break;
	case (UINT)SCRIPT_TYPE::LYCHEESCRIPT:
		return new CLycheeScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::JUMPSTATE:
		return new CJumpState;
		break;
	case (UINT)SCRIPT_TYPE::RUNSTATE:
		return new CRunState;
		break;
	case (UINT)SCRIPT_TYPE::STATEMACHINE:
		return new StateMachine;
		break;
	}
	return nullptr;
}

const wchar_t * ScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMMOVESCRIPT:
		return L"CCamMoveScript";
		break;

	case SCRIPT_TYPE::ENEMY:
		return L"CEnemy";
		break;

	case SCRIPT_TYPE::LYCHEESCRIPT:
		return L"CLycheeScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::JUMPSTATE:
		return L"JumpState";
		break;

	case SCRIPT_TYPE::RUNSTATE:
		return L"RunState";
		break;

	case SCRIPT_TYPE::STATEMACHINE:
		return L"StateMachine";
		break;

	}
	return nullptr;
}