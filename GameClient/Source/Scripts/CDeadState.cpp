#include "pch.h"
#include "CDeadState.h"

#include "AssetMgr.h"
#include "GameObject.h"
#include "TimeMgr.h"
#include "SceneFlowMgr.h"

CDeadState::CDeadState()
	: PlayerState((UINT)SCRIPT_TYPE::DEADSTATE, nullptr, PLAYER_STATE_ID::DEAD)
{
}

CDeadState::CDeadState(CPlayerScript* _Owner)
	: PlayerState((UINT)SCRIPT_TYPE::DEADSTATE, _Owner, PLAYER_STATE_ID::DEAD)
{

}

CDeadState::~CDeadState()
{
}


void CDeadState::Enter(PLAYER_STATE_ID _prev)
{
	Ptr<ASound> pCoinSFX = FIND(ASound, L"dead");
	if (pCoinSFX != nullptr)
		pCoinSFX->Play(1, 0.7f, true);

	GetOwner()->FlipbookRender()->Play((int)PLAYER_STATE_ID::DEAD, 5.f, 0);
    m_fDelayTimer = 0.f;
	m_bDelayStarted = false;
}

void CDeadState::Tick()
{
    if (GetOwner()->FlipbookRender()->IsAnimationComplete())
	{
		if (!m_bDelayStarted)
		{
			m_bDelayStarted = true;
			m_fDelayTimer = 0.f;
		}

		m_fDelayTimer += DT;

		if (m_fDelayTimer >= m_fDelayDuration)
		{
			SceneFlowMgr::GetInst()->ExecuteAction(SCENE_FLOW_ACTION::GO_SCORE);
		}
	}
}

void CDeadState::Exit(PLAYER_STATE_ID _Next)
{
}
