#pragma once
#include "CPlayerScript.h"

class RunState :
	public PlayerState
{
public:
	void Enter(PLAYERSTATEID _prev) override;
	void Tick(float dt) override;
	void Exit(PLAYERSTATEID _Next) override;

public:
	RunState(CPlayerScript* _Owner);
	virtual ~RunState();
};

