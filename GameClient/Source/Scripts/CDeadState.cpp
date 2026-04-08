#include "pch.h"
#include "CDeadState.h"

#include "GameObject.h"
#include "SceneFlowMgr.h"

CDeadState::CDeadState()
	: PlayerState((UINT)SCRIPT_TYPE::DEADSTATE, nullptr, PLAYER_STATE_ID::DEAD)
{
}

CDeadState::CDeadState(CPlayerScript* _Owner)
	: PlayerState((UINT)SCRIPT_TYPE::DEADSTATE, _Owner, PLAYER_STATE_ID::DEAD)
{

}

CDeadState::~CDeadState()
{
}


void CDeadState::Enter(PLAYER_STATE_ID _prev)
{
	GetOwner()->FlipbookRender()->Play((int)PLAYER_STATE_ID::FALL, 8.f, 0);
}

void CDeadState::Tick()
{
	if (GetOwner()->FlipbookRender()->IsAnimationComplete())
	{
		SceneFlowMgr::GetInst()->ExecuteAction(SCENE_FLOW_ACTION::GO_LOBBY);
	}
}

void CDeadState::Exit(PLAYER_STATE_ID _Next)
{
}
