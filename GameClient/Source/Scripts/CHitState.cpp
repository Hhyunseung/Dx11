#include "pch.h"
#include "CHitState.h"

#include "GameObject.h"

CHitState::CHitState()
	: PlayerState((UINT)SCRIPT_TYPE::HITSTATE, nullptr, PLAYER_STATE_ID::HIT)
{
}

CHitState::CHitState(CPlayerScript* _Owner)
	: PlayerState((UINT)SCRIPT_TYPE::HITSTATE, _Owner, PLAYER_STATE_ID::HIT)
{
}

CHitState::~CHitState()
{
}

void CHitState::Enter(PLAYER_STATE_ID _prev)
{
	GetOwner()->SetIsJump(false);
	GetOwner()->SetIsInvincible(true);

	GetOwner()->FlipbookRender()->Play((int)PLAYER_STATE_ID::HIT, 6.f, 1);
}

void CHitState::Tick()
{ 
	if ((int)PLAYER_STATE_ID::HIT && GetOwner()->FlipbookRender()->IsAnimationComplete())
	{
		GetOwner()->ChangeState(PLAYER_STATE_ID::RUN);
	}
}

void CHitState::Exit(PLAYER_STATE_ID _Next)
{

}
