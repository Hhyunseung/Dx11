#pragma once
#include "CPlayerScript.h"

class CJumpState :
    public PlayerState
{
public:
    void Enter(PLAYER_STATE_ID _prev) override;
    void Tick();
    void Exit(PLAYER_STATE_ID _Next) override;

	CLONE(CJumpState);

public:
    CJumpState();
    CJumpState(CPlayerScript* _Owner);
    virtual ~CJumpState();
};

