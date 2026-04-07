#include "pch.h"
#include "ScriptMgr.h"

#include "Scripts/CBGScrollScript.h"
#include "Scripts/CCamMoveScript.h"
#include "Scripts/CCookieSkillScript.h"
#include "Scripts/CDoubleJumpState.h"
#include "Scripts/CEffectScript.h"
#include "Scripts/CEnemy.h"
#include "Scripts/CGamePlaySpawnScript.h"
#include "Scripts/CGamePlayUIScript.h"
#include "Scripts/CHitState.h"
#include "Scripts/CJellyScript.h"
#include "Scripts/CJumpButtonScript.h"
#include "Scripts/CJumpState.h"
#include "Scripts/CLandState.h"
#include "Scripts/CLinearObstacle.h"
#include "Scripts/CLycheeScript.h"
#include "Scripts/CMissileScript.h"
#include "Scripts/CMonsterScript.h"
#include "Scripts/CMovingPlatformScirpt.h"
#include "Scripts/CObstructScript.h"
#include "Scripts/CParabolicObstacle.h"
#include "Scripts/CPendulumObstacle.h"
#include "Scripts/CPlayerScript.h"
#include "Scripts/CRunState.h"
#include "Scripts/CScrollScript.h"
#include "Scripts/CSlideButtonScript.h"
#include "Scripts/CSlideState.h"
#include "Scripts/CStateMachine.h"
#include "Scripts/CStaticObstacle.h"
#include "Scripts/CStaticPlatformScript.h"
#include "Scripts/CTimeKeeperScript.h"
#include "Scripts/CUIButtonScript.h"
#include "Scripts/CWorldScrollScript.h"

void ScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBGScrollScript");
	_vec.push_back(L"CCamMoveScript");
	_vec.push_back(L"CCookieSkillScript");
	_vec.push_back(L"CDoubleJumpState");
	_vec.push_back(L"CEffectScript");
	_vec.push_back(L"CEnemy");
	_vec.push_back(L"CGamePlaySpawnScript");
	_vec.push_back(L"CGamePlayUIScript");
	_vec.push_back(L"CHitState");
	_vec.push_back(L"CJellyScript");
	_vec.push_back(L"CJumpButtonScript");
	_vec.push_back(L"CJumpState");
	_vec.push_back(L"CLandState");
	_vec.push_back(L"CLinearObstacle");
	_vec.push_back(L"CLycheeScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMovingPlatformScirpt");
	_vec.push_back(L"CObstructScript");
	_vec.push_back(L"CParabolicObstacle");
	_vec.push_back(L"CPendulumObstacle");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CRunState");
	_vec.push_back(L"CScrollScript");
	_vec.push_back(L"CSlideButtonScript");
	_vec.push_back(L"CSlideState");
	_vec.push_back(L"CStateMachine");
	_vec.push_back(L"CStaticObstacle");
	_vec.push_back(L"CStaticPlatformScript");
	_vec.push_back(L"CTimeKeeperScript");
	_vec.push_back(L"CUIButtonScript");
	_vec.push_back(L"CWorldScrollScript");
}

CScript * ScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBGScrollScript" == _strScriptName)
		return new CBGScrollScript;
	if (L"CCamMoveScript" == _strScriptName)
		return new CCamMoveScript;
	if (L"CCookieSkillScript" == _strScriptName)
		return new CCookieSkillScript;
	if (L"CDoubleJumpState" == _strScriptName)
		return new CDoubleJumpState;
	if (L"CEffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"CEnemy" == _strScriptName)
		return new CEnemy;
	if (L"CGamePlaySpawnScript" == _strScriptName)
		return new CGamePlaySpawnScript;
	if (L"CGamePlayUIScript" == _strScriptName)
		return new CGamePlayUIScript;
	if (L"CHitState" == _strScriptName)
		return new CHitState;
	if (L"CJellyScript" == _strScriptName)
		return new CJellyScript;
	if (L"CJumpButtonScript" == _strScriptName)
		return new CJumpButtonScript;
	if (L"CJumpState" == _strScriptName)
		return new CJumpState;
	if (L"CLandState" == _strScriptName)
		return new CLandState;
	if (L"CLinearObstacle" == _strScriptName)
		return new CLinearObstacle;
	if (L"CLycheeScript" == _strScriptName)
		return new CLycheeScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMovingPlatformScirpt" == _strScriptName)
		return new CMovingPlatformScirpt;
	if (L"CObstructScript" == _strScriptName)
		return new CObstructScript;
	if (L"CParabolicObstacle" == _strScriptName)
		return new CParabolicObstacle;
	if (L"CPendulumObstacle" == _strScriptName)
		return new CPendulumObstacle;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CRunState" == _strScriptName)
		return new CRunState;
	if (L"CScrollScript" == _strScriptName)
		return new CScrollScript;
	if (L"CSlideButtonScript" == _strScriptName)
		return new CSlideButtonScript;
	if (L"CSlideState" == _strScriptName)
		return new CSlideState;
	if (L"CStateMachine" == _strScriptName)
		return new CStateMachine;
	if (L"CStaticObstacle" == _strScriptName)
		return new CStaticObstacle;
	if (L"CStaticPlatformScript" == _strScriptName)
		return new CStaticPlatformScript;
	if (L"CTimeKeeperScript" == _strScriptName)
		return new CTimeKeeperScript;
	if (L"CUIButtonScript" == _strScriptName)
		return new CUIButtonScript;
	if (L"CWorldScrollScript" == _strScriptName)
		return new CWorldScrollScript;
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
	case (UINT)SCRIPT_TYPE::COOKIESKILLSCRIPT:
		return new CCookieSkillScript;
		break;
	case (UINT)SCRIPT_TYPE::DOUBLEJUMPSTATE:
		return new CDoubleJumpState;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCRIPT:
		return new CEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::ENEMY:
		return new CEnemy;
		break;
	case (UINT)SCRIPT_TYPE::GAMEPLAYSPAWNSCRIPT:
		return new CGamePlaySpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::GAMEPLAYUISCRIPT:
		return new CGamePlayUIScript;
		break;
	case (UINT)SCRIPT_TYPE::HITSTATE:
		return new CHitState;
		break;
	case (UINT)SCRIPT_TYPE::JELLYSCRIPT:
		return new CJellyScript;
		break;
	case (UINT)SCRIPT_TYPE::JUMPBUTTONSCRIPT:
		return new CJumpButtonScript;
		break;
	case (UINT)SCRIPT_TYPE::JUMPSTATE:
		return new CJumpState;
		break;
	case (UINT)SCRIPT_TYPE::LANDSTATE:
		return new CLandState;
		break;
	case (UINT)SCRIPT_TYPE::LINEAROBSTACLE:
		return new CLinearObstacle;
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
	case (UINT)SCRIPT_TYPE::OBSTRUCTSCRIPT:
		return new CObstructScript;
		break;
	case (UINT)SCRIPT_TYPE::PARABOLICOBSTACLE:
		return new CParabolicObstacle;
		break;
	case (UINT)SCRIPT_TYPE::PENDULUMOBSTACLE:
		return new CPendulumObstacle;
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
	case (UINT)SCRIPT_TYPE::SLIDEBUTTONSCRIPT:
		return new CSlideButtonScript;
		break;
	case (UINT)SCRIPT_TYPE::SLIDESTATE:
		return new CSlideState;
		break;
	case (UINT)SCRIPT_TYPE::STATEMACHINE:
		return new CStateMachine;
		break;
	case (UINT)SCRIPT_TYPE::STATICOBSTACLE:
		return new CStaticObstacle;
		break;
	case (UINT)SCRIPT_TYPE::STATICPLATFORMSCRIPT:
		return new CStaticPlatformScript;
		break;
	case (UINT)SCRIPT_TYPE::TIMEKEEPERSCRIPT:
		return new CTimeKeeperScript;
		break;
	case (UINT)SCRIPT_TYPE::UIBUTTONSCRIPT:
		return new CUIButtonScript;
		break;
	case (UINT)SCRIPT_TYPE::WORLDSCROLLSCRIPT:
		return new CWorldScrollScript;
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

	case SCRIPT_TYPE::COOKIESKILLSCRIPT:
		return L"CCookieSkillScript";
		break;

	case SCRIPT_TYPE::DOUBLEJUMPSTATE:
		return L"CDoubleJumpState";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::ENEMY:
		return L"CEnemy";
		break;

	case SCRIPT_TYPE::GAMEPLAYSPAWNSCRIPT:
		return L"CGamePlaySpawnScript";
		break;

	case SCRIPT_TYPE::GAMEPLAYUISCRIPT:
		return L"CGamePlayUIScript";
		break;

	case SCRIPT_TYPE::HITSTATE:
		return L"CHitState";
		break;

	case SCRIPT_TYPE::JELLYSCRIPT:
		return L"CJellyScript";
		break;

	case SCRIPT_TYPE::JUMPBUTTONSCRIPT:
		return L"CJumpButtonScript";
		break;

	case SCRIPT_TYPE::JUMPSTATE:
		return L"CJumpState";
		break;

	case SCRIPT_TYPE::LANDSTATE:
		return L"CLandState";
		break;

	case SCRIPT_TYPE::LINEAROBSTACLE:
		return L"CLinearObstacle";
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

	case SCRIPT_TYPE::OBSTRUCTSCRIPT:
		return L"CObstructScript";
		break;

	case SCRIPT_TYPE::PARABOLICOBSTACLE:
		return L"CParabolicObstacle";
		break;

	case SCRIPT_TYPE::PENDULUMOBSTACLE:
		return L"CPendulumObstacle";
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

	case SCRIPT_TYPE::SLIDEBUTTONSCRIPT:
		return L"CSlideButtonScript";
		break;

	case SCRIPT_TYPE::SLIDESTATE:
		return L"CSlideState";
		break;

	case SCRIPT_TYPE::STATEMACHINE:
		return L"CStateMachine";
		break;

	case SCRIPT_TYPE::STATICOBSTACLE:
		return L"CStaticObstacle";
		break;

	case SCRIPT_TYPE::STATICPLATFORMSCRIPT:
		return L"CStaticPlatformScript";
		break;

	case SCRIPT_TYPE::TIMEKEEPERSCRIPT:
		return L"CTimeKeeperScript";
		break;

	case SCRIPT_TYPE::UIBUTTONSCRIPT:
		return L"CUIButtonScript";
		break;

	case SCRIPT_TYPE::WORLDSCROLLSCRIPT:
		return L"CWorldScrollScript";
		break;

	}
	return nullptr;
}