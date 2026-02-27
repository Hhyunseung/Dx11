#include "pch.h"
#include "StateMachine.h"


StateMachine::StateMachine()
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::AddState(Ptr<PlayerState> _State)
{
	// 상태 추가
	//m_arrState[(UINT)PLAY]
}

void StateMachine::StartState(PLAYERSTATEID _Start)
{
}

void StateMachine::Tick(float dt)
{
}

void StateMachine::ChangeState(PLAYERSTATEID _Next)
{
}
