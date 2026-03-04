#pragma once
#include "CPlayerScript.h"

class RunState :
	public PlayerState
{
public:
	void Enter(PLAYER_STATE_ID _prev) override;
	void Tick();
	void Exit(PLAYER_STATE_ID _Next) override;

	CLONE(RunState);

public:
	RunState(CPlayerScript* _Owner);
	virtual ~RunState();
};

