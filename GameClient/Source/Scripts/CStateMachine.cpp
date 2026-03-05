#include "pch.h"
#include "CStateMachine.h"

#include "CPlayerScript.h"


CStateMachine::CStateMachine()
	: CScript(SCRIPT_TYPE::STATEMACHINE)
	, m_CurStateId(PLAYER_STATE_ID::END)
	, m_NextStateId(PLAYER_STATE_ID::END)
	, m_Change(false)
{
}

CStateMachine::~CStateMachine()
{
}

void CStateMachine::AddState(Ptr<PlayerState> _State)
{
	// 상태 추가
	PLAYER_STATE_ID id = _State->GetId();
	m_arrState[(UINT)id] = _State;
}

void CStateMachine::StartState(PLAYER_STATE_ID _Start)
{
	// 시작하려는 상태가 등록되어 있는지 확인
	assert(m_arrState[(UINT)_Start] != nullptr);

	// 시작하려는 상태로 변경
	m_CurStateId = _Start;
	m_CurState = m_arrState[(UINT)m_CurStateId];
	m_CurState->Enter(m_CurStateId);
}

void CStateMachine::Tick()
{
	// 현재 상태가 존재하는지 확인
	assert(m_CurState != nullptr);
	
	m_CurState->Tick();

	if (m_Change)
	{
		RequestChange(m_NextStateId);
		m_Change = false;
	}
}

void CStateMachine::ChangeState(PLAYER_STATE_ID _NextId)
{
	if (m_CurStateId == _NextId)
		return;

	m_NextStateId = _NextId;
	m_Change = true;
}

void CStateMachine::RequestChange(PLAYER_STATE_ID _NextId)
{
	assert(m_arrState[(UINT)_NextId] != nullptr);

	// 현재 상태에서 나가기
	m_CurState->Exit(_NextId);

	// 다음 상태로 변경
	m_CurStateId = _NextId;
	m_CurState = m_arrState[(UINT)m_CurStateId];
	m_CurState->Enter(m_CurStateId);
}
