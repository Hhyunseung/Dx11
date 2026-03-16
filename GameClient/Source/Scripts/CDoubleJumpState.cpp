#include "pch.h"
#include "CDoubleJumpState.h"

#include "GameObject.h"

CDoubleJumpState::CDoubleJumpState()
	: PlayerState((UINT)SCRIPT_TYPE::DOUBLEJUMPSTATE, nullptr, PLAYER_STATE_ID::DOUBLE_JUMP)

{
}

CDoubleJumpState::CDoubleJumpState(CPlayerScript* _Owner)
	: PlayerState((UINT)SCRIPT_TYPE::DOUBLEJUMPSTATE, _Owner, PLAYER_STATE_ID::DOUBLE_JUMP)
{
}

CDoubleJumpState::~CDoubleJumpState()
{
}


void CDoubleJumpState::Enter(PLAYER_STATE_ID _prev)
{
	GetOwner()->SetIsLand(false);
	GetOwner()->SetIsDoubleJump(true);

	GetOwner()->FlipbookRender()->Play(2, 8.f, 1);
}

void CDoubleJumpState::Tick()
{
	if (GetOwner()->GetIsLand())
	{
		GetOwner()->ChangeState(PLAYER_STATE_ID::LAND);
	}
}

void CDoubleJumpState::Exit(PLAYER_STATE_ID _Next)
{
	GetOwner()->SetIsDoubleJump(false);
}
