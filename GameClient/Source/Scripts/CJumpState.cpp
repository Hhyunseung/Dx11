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
	GetOwner()->FlipbookRender()->Play(2, 8.f, 1);
}

void CJumpState::Tick()
{

}

void CJumpState::Exit(PLAYER_STATE_ID _Next)
{
}
