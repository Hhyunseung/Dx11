#include "pch.h"
#include "JumpState.h"


JumpState::JumpState(CPlayerScript* _Owner)
	: PlayerState(_Owner, PLAYER_STATE_ID::JUMP)
{
}

JumpState::~JumpState()
{
}


void JumpState::Enter(PLAYER_STATE_ID _prev)
{
}

void JumpState::Tick(float dt)
{
}

void JumpState::Exit(PLAYER_STATE_ID _Next)
{
}
