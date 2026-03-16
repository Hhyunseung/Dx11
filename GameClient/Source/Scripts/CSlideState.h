#pragma once
#include "CPlayerScript.h"
class CSlideState :
    public PlayerState
{
public:
    void Enter(PLAYER_STATE_ID _prev) override;
    void Tick();
    void Exit(PLAYER_STATE_ID _Next) override;

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _File) override {}

    CLONE(CSlideState);

public:
    CSlideState();
    CSlideState(CPlayerScript* _Owner);
    virtual ~CSlideState();
};

