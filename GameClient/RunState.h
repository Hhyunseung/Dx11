#pragma once
#include "CPlayerScript.h"

class RunState :
	public PlayerState
{
public:
	void Enter(PLAYER_STATE_ID _prev) override;
	void Tick(float dt) override;
	void Exit(PLAYER_STATE_ID _Next) override;

public:
	RunState(CPlayerScript* _Owner);
	virtual ~RunState();
};

