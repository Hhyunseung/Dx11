#include "pch.h"
#include "JumpState.h"

#include "CFlipbookRender.h"

JumpState::JumpState(CPlayerScript* _Owner)
	: PlayerState(_Owner, PLAYER_STATE_ID::JUMP)
{
}

JumpState::~JumpState()
{
}


void JumpState::Enter(PLAYER_STATE_ID _prev)
{
	GetOwner()->FlipbookRender()->Play(2, 8.f, 1);
}

void JumpState::Tick()
{

}

void JumpState::Exit(PLAYER_STATE_ID _Next)
{
}
