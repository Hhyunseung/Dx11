#pragma once
#include "CPlayerScript.h"
class CDeadState :
    public PlayerState
{
public:
	void Enter(PLAYER_STATE_ID _prev) override;
	void Tick();
	void Exit(PLAYER_STATE_ID _Next) override;

private:
	float m_fDelayTimer = 0.f;
	bool  m_bDelayStarted = false;
	float m_fDelayDuration = 3.f; 

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CDeadState);

public:
	CDeadState();
	CDeadState(CPlayerScript* _Owner);
	virtual ~CDeadState();
};

