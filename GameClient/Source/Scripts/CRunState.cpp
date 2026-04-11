#include "pch.h"
#include "CRunState.h"

#include "TimeMgr.h"
#include "KeyMgr.h"
#include "GameObject.h"
#include "CTransform.h"


CRunState::CRunState()
	: PlayerState((UINT)SCRIPT_TYPE::RUNSTATE, nullptr, PLAYER_STATE_ID::RUN)
	, m_WasBoosting(false)
{
}

CRunState::CRunState(CPlayerScript* _Owner)
	: PlayerState((UINT)SCRIPT_TYPE::RUNSTATE, _Owner, PLAYER_STATE_ID::RUN)
	, m_WasBoosting(false)
{
}

CRunState::~CRunState()
{
}

void CRunState::Enter(PLAYER_STATE_ID _prev)
{
	m_WasBoosting = GetOwner()->GetIsBoost();

	if (m_WasBoosting)
		GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::BOOST_RUN, 12.f, -1);
	else
		GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::RUN, 8.f, -1);
}

void CRunState::Tick()
{
	bool bBoosting = GetOwner()->GetIsBoost();

	if (bBoosting == m_WasBoosting)
		return;

	m_WasBoosting = bBoosting;

	if (bBoosting)
		GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::BOOST_RUN, 12.f, -1);
	else
		GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::RUN, 8.f, -1);
}

void CRunState::Exit(PLAYER_STATE_ID _Next)
{

}
