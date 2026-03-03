#include "pch.h"
#include "RunState.h"



RunState::RunState(CPlayerScript* _Owner)
	: PlayerState(_Owner, PLAYER_STATE_ID::RUN)
{
}

RunState::~RunState()
{
}

void RunState::Enter(PLAYER_STATE_ID _prev)
{
}

void RunState::Tick(float dt)
{
}

void RunState::Exit(PLAYER_STATE_ID _Next)
{
}
