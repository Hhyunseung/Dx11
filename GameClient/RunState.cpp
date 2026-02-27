#include "pch.h"
#include "RunState.h"



RunState::RunState(CPlayerScript* _Owner)
	: PlayerState(_Owner, PLAYERSTATEID::RUN)
{
}

RunState::~RunState()
{
}

void RunState::Enter(PLAYERSTATEID _prev)
{
}

void RunState::Tick(float dt)
{
}

void RunState::Exit(PLAYERSTATEID _Next)
{
}
