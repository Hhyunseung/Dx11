#include "pch.h"
#include "ScriptMgr.h"

#include "Scripts/CBGScrollScript.h"
#include "Scripts/CCamMoveScript.h"
#include "Scripts/CDoubleJumpState.h"
#include "Scripts/CEnemy.h"
#include "Scripts/CJumpState.h"
#include "Scripts/CLandState.h"
#include "Scripts/CLycheeScript.h"
#include "Scripts/CMissileScript.h"
#include "Scripts/CMonsterScript.h"
#include "Scripts/CMovingPlatformScirpt.h"
#include "Scripts/CPlayerScript.h"
#include "Scripts/CRunState.h"
#include "Scripts/CScrollScript.h"
#include "Scripts/CStateMachine.h"

void ScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBGScrollScript");
	_vec.push_back(L"CCamMoveScript");
	_vec.push_back(L"CDoubleJumpState");
	_vec.push_back(L"CEnemy");
	_vec.push_back(L"CJumpState");
	_vec.push_back(L"CLandState");
	_vec.push_back(L"CLycheeScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMovingPlatformScirpt");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CRunState");
	_vec.push_back(L"CScrollScript");
	_vec.push_back(L"CStateMachine");
}

CScript * ScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBGScrollScript" == _strScriptName)
		return new CBGScrollScript;
	if (L"CCamMoveScript" == _strScriptName)
		return new CCamMoveScript;
	if (L"CDoubleJumpState" == _strScriptName)
		return new CDoubleJumpState;
	if (L"CEnemy" == _strScriptName)
		return new CEnemy;
	if (L"CJumpState" == _strScriptName)
		return new CJumpState;
	if (L"CLandState" == _strScriptName)
		return new CLandState;
	if (L"CLycheeScript" == _strScriptName)
		return new CLycheeScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMovingPlatformScirpt" == _strScriptName)
		return new CMovingPlatformScirpt;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CRunState" == _strScriptName)
		return new CRunState;
	if (L"CScrollScript" == _strScriptName)
		return new CScrollScript;
	if (L"CStateMachine" == _strScriptName)
		return new CStateMachine;
	return nullptr;
}

CScript * ScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BGSCROLLSCRIPT:
		return new CBGScrollScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMMOVESCRIPT:
		return new CCamMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::DOUBLEJUMPSTATE:
		return new CDoubleJumpState;
		break;
	case (UINT)SCRIPT_TYPE::ENEMY:
		return new CEnemy;
		break;
	case (UINT)SCRIPT_TYPE::JUMPSTATE:
		return new CJumpState;
		break;
	case (UINT)SCRIPT_TYPE::LANDSTATE:
		return new CLandState;
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
	case (UINT)SCRIPT_TYPE::MOVINGPLATFORMSCIRPT:
		return new CMovingPlatformScirpt;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::RUNSTATE:
		return new CRunState;
		break;
	case (UINT)SCRIPT_TYPE::SCROLLSCRIPT:
		return new CScrollScript;
		break;
	case (UINT)SCRIPT_TYPE::STATEMACHINE:
		return new CStateMachine;
		break;
	}
	return nullptr;
}

const wchar_t * ScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BGSCROLLSCRIPT:
		return L"CBGScrollScript";
		break;

	case SCRIPT_TYPE::CAMMOVESCRIPT:
		return L"CCamMoveScript";
		break;

	case SCRIPT_TYPE::DOUBLEJUMPSTATE:
		return L"CDoubleJumpState";
		break;

	case SCRIPT_TYPE::ENEMY:
		return L"CEnemy";
		break;

	case SCRIPT_TYPE::JUMPSTATE:
		return L"CJumpState";
		break;

	case SCRIPT_TYPE::LANDSTATE:
		return L"CLandState";
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

	case SCRIPT_TYPE::MOVINGPLATFORMSCIRPT:
		return L"CMovingPlatformScirpt";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::RUNSTATE:
		return L"CRunState";
		break;

	case SCRIPT_TYPE::SCROLLSCRIPT:
		return L"CScrollScript";
		break;

	case SCRIPT_TYPE::STATEMACHINE:
		return L"CStateMachine";
		break;

	}
	return nullptr;
}