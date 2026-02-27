#pragma once
#include "CPlayerScript.h"

class JumpState :
    public PlayerState
{
public:
    void Enter(PLAYERSTATEID _prev) override;
    void Tick(float dt) override;
    void Exit(PLAYERSTATEID _Next) override;

public:
    JumpState(CPlayerScript* _Owner);
    virtual ~JumpState();
};

