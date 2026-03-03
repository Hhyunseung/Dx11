#pragma once
#include "CPlayerScript.h"

class JumpState :
    public PlayerState
{
public:
    void Enter(PLAYER_STATE_ID _prev) override;
    void Tick(float dt) override;
    void Exit(PLAYER_STATE_ID _Next) override;

public:
    JumpState(CPlayerScript* _Owner);
    virtual ~JumpState();
};

