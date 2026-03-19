#include "pch.h"
#include "CSlideState.h"

#include "GameObject.h"

CSlideState::CSlideState()
	: PlayerState((UINT)SCRIPT_TYPE::SLIDESTATE, nullptr, PLAYER_STATE_ID::SLIDE)
{
}

CSlideState::CSlideState(CPlayerScript* _Owner)
	: PlayerState((UINT)SCRIPT_TYPE::SLIDESTATE, _Owner, PLAYER_STATE_ID::SLIDE)
{
}

CSlideState::~CSlideState()
{
}

void CSlideState::Enter(PLAYER_STATE_ID _prev)
{
	GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::SLIDE, 8.f, -1);
}

void CSlideState::Tick()
{
}

void CSlideState::Exit(PLAYER_STATE_ID _Next)
{

}

