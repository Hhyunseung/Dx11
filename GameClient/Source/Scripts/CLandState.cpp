#include "pch.h"
#include "CLandState.h"

#include "GameObject.h"

CLandState::CLandState()
	: PlayerState((UINT)SCRIPT_TYPE::LANDSTATE, nullptr, PLAYER_STATE_ID::LAND)
	, m_LandTime(0.f)
{
}

CLandState::CLandState(CPlayerScript* _Owner)
	: PlayerState((UINT)SCRIPT_TYPE::LANDSTATE, _Owner, PLAYER_STATE_ID::LAND)
	, m_LandTime(0.f)
{
}

CLandState::~CLandState()
{
}

void CLandState::Enter(PLAYER_STATE_ID _prev)
{
	m_LandTime = 0.f;

	if (PLAYER_STATE_ID::JUMP == _prev || PLAYER_STATE_ID::DOUBLE_JUMP == _prev)
	{
		GetOwner()->SetIsLand(true);
		GetOwner()->SetIsJump(false);
		GetOwner()->SetIsDoubleJump(false);

		GetOwner()->FlipbookRender()->Play(3, 8.f, 1);
	}
}

void CLandState::Tick()
{
	// 0.3초 정도 땅에 닿아있는 상태 유지하다가 다시 달리는 상태로
	m_LandTime += g_Global.DeltaTime;

	if (m_LandTime >= 0.2f)
	{
		GetOwner()->ChangeState(PLAYER_STATE_ID::RUN);
	}
}

void CLandState::Exit(PLAYER_STATE_ID _Next)
{
}
