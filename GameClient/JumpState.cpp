#include "pch.h"
#include "JumpState.h"


JumpState::JumpState(CPlayerScript* _Owner)
	: PlayerState(_Owner, PLAYERSTATEID::JUMP)
{
}

JumpState::~JumpState()
{
}


void JumpState::Enter(PLAYERSTATEID _prev)
{
}

void JumpState::Tick(float dt)
{
}

void JumpState::Exit(PLAYERSTATEID _Next)
{
}
