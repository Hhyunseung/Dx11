#pragma once
#include "CScript.h"

class CStateMachine
	: public CScript
{
private:
	friend class PlayerState;

private:
	Ptr<PlayerState>	m_arrState[(UINT)PLAYER_STATE_ID::END];

	Ptr<PlayerState>	m_CurState;
	PLAYER_STATE_ID		m_CurStateId;
	PLAYER_STATE_ID		m_NextStateId;

	bool				m_Change; // 상태 변경 요청 여부

public:
	void AddState(Ptr<PlayerState> _State);
	void StartState(PLAYER_STATE_ID _Start);
	void Tick();
	void ChangeState(PLAYER_STATE_ID _NextId);

private:
	void RequestChange(PLAYER_STATE_ID _NextId);

	CLONE(CStateMachine);

public:
	CStateMachine();
	~CStateMachine();
};

