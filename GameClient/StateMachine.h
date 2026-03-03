#pragma once


class StateMachine
{
private:
	friend class PlayerState;

private:
	Ptr<PlayerState>	m_arrState[(UINT)PLAYER_STATE_ID::END];

	Ptr<PlayerState>	m_CurState;

public:
	void AddState(Ptr<PlayerState> _State);
	void StartState(PLAYER_STATE_ID _Start);
	void Tick(float dt);
	void ChangeState(PLAYER_STATE_ID _Next);

private:
	void RequestChange();

public:
	StateMachine();
	~StateMachine();
};

