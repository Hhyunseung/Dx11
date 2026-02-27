#pragma once


class StateMachine
{
private:
	friend class PlayerState;

private:
	Ptr<PlayerState>	m_arrState[(UINT)PLAYERSTATEID::END];

	Ptr<PlayerState>	m_CurState;

public:
	void AddState(Ptr<PlayerState> _State);
	void StartState(PLAYERSTATEID _Start);
	void Tick(float dt);
	void ChangeState(PLAYERSTATEID _Next);

public:
	StateMachine();
	~StateMachine();
};

