#pragma once
#include "CPlayerScript.h"

class JumpState :
    public PlayerState
{
public:
    void Enter(PLAYER_STATE_ID _prev) override;
    void Tick();
    void Exit(PLAYER_STATE_ID _Next) override;

	CLONE(JumpState);

public:
    JumpState(CPlayerScript* _Owner);
    virtual ~JumpState();
};

