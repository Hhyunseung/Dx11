#pragma once
#include "CPlayerScript.h"

class CRunState :
	public PlayerState
{
public:
	void Enter(PLAYER_STATE_ID _prev) override;
	void Tick();
	void Exit(PLAYER_STATE_ID _Next) override;

	CLONE(CRunState);

public:
	CRunState();
	CRunState(CPlayerScript* _Owner);
	virtual ~CRunState();
};

