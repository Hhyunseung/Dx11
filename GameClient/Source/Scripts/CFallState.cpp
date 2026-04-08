#include "pch.h"
#include "CFallState.h"

#include "GameObject.h"

CFallState::CFallState()
	: PlayerState((UINT)SCRIPT_TYPE::FALLSTATE, nullptr, PLAYER_STATE_ID::FALL)
{
}

CFallState::CFallState(CPlayerScript* _Owner)
	: PlayerState((UINT)SCRIPT_TYPE::FALLSTATE, _Owner, PLAYER_STATE_ID::FALL)
{
}

CFallState::~CFallState()
{
}


void CFallState::Enter(PLAYER_STATE_ID _prev)
{
	GetOwner()->FlipbookRender()->Play((int)PLAYER_STATE_ID::FALL, 8.f, -1);
}

void CFallState::Tick()
{
}

void CFallState::Exit(PLAYER_STATE_ID _Next)
{
}

