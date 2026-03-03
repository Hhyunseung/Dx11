#include "pch.h"
#include "StateMachine.h"

#include "CPlayerScript.h"


StateMachine::StateMachine()
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::AddState(Ptr<PlayerState> _State)
{
	// 상태 추가
	PLAYER_STATE_ID id = _State->GetId();
	m_arrState[(UINT)id] = _State;
}

void StateMachine::StartState(PLAYER_STATE_ID _Start)
{
	// 시작하려는 상태가 등록되어 있는지 확인
	assert(m_arrState[(UINT)_Start] != nullptr);

	// 시작하려는 상태로 변경
	m_CurState = m_arrState[(UINT)_Start];
	m_CurState->Enter(_Start);
}

void StateMachine::Tick(float dt)
{
	// 현재 상태가 존재하는지 확인
	assert(m_CurState != nullptr);
	
	m_CurState->Tick(dt);
}

void StateMachine::ChangeState(PLAYER_STATE_ID _Next)
{
}

void StateMachine::RequestChange()
{

}
