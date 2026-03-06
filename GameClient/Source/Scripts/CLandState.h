#pragma once
#include "CPlayerScript.h"

class CLandState :
	public PlayerState
{
private:
	float m_LandTime; // 착지 후 경과 시간

public:
	void Enter(PLAYER_STATE_ID _prev) override;
	void Tick();
	void Exit(PLAYER_STATE_ID _Next) override;

	CLONE(CLandState);

public:
	CLandState();
	CLandState(CPlayerScript* _Owner);
	virtual ~CLandState();
};

