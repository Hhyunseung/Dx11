#include "pch.h"
#include "CJumpState.h"

#include "CFlipbookRender.h"

CJumpState::CJumpState()
	: PlayerState((UINT)SCRIPT_TYPE::JUMPSTATE, nullptr, PLAYER_STATE_ID::JUMP)
{
}

CJumpState::CJumpState(CPlayerScript* _Owner)
	: PlayerState((UINT)SCRIPT_TYPE::JUMPSTATE, _Owner, PLAYER_STATE_ID::JUMP)
{
}

CJumpState::~CJumpState()
{
}


void CJumpState::Enter(PLAYER_STATE_ID _prev)
{
	GetOwner()->SetIsLand(false);
	GetOwner()->SetIsJump(true);

	GetOwner()->FlipbookRender()->Play(1, 8.f, 1);
}

void CJumpState::Tick()
{
	if (GetOwner()->GetIsLand() && !GetOwner()->GetIsDead())
	{
		GetOwner()->ChangeState(PLAYER_STATE_ID::LAND);
	}


	//if (!GetOwner()->GetIsDoubleJump())
	//{
	//	GetOwner()->SetIsJump(false);
	//	GetOwner()->SetIsDoubleJump(true);
	//	GetOwner()->FlipbookRender()->Play(2, 8.f, 1);

	//	//GetOwner()->ChangeState(PLAYER_STATE_ID::DOUBLE_JUMP);
	//}
}

void CJumpState::Exit(PLAYER_STATE_ID _Next)
{
	GetOwner()->SetIsJump(false);
}
