#pragma once
#include "CPlayerScript.h"
class CFallState :
    public PlayerState
{
public:
	void Enter(PLAYER_STATE_ID _prev) override;
	void Tick();
	void Exit(PLAYER_STATE_ID _Next) override;

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CFallState);

public:
	CFallState();
	CFallState(CPlayerScript* _Owner);
	virtual ~CFallState();
};

