#pragma once
#include "CScript.h"

#include "CMissileScript.h"
#include "StateMachine.h"


class CPlayerScript;

class PlayerState 
	: public CScript
{
private:
	CPlayerScript*      m_Owner;
	PLAYER_STATE_ID       m_Id;

public:
	CPlayerScript* GetOwner() { return m_Owner; }
	PLAYER_STATE_ID GetId() { return m_Id; }

public:
	virtual void Enter(PLAYER_STATE_ID _Prev) = 0;
	virtual void Exit(PLAYER_STATE_ID _Next) = 0;

public:
	PlayerState(CPlayerScript* _Owner, PLAYER_STATE_ID _Id) 
		: m_Owner(_Owner), m_Id(_Id) {}
	virtual ~PlayerState() {}
};





class CPlayerScript :
    public CScript
{
private:
    Ptr<GameObject>		m_Target;

	CCollider2D*		m_BodyCollider; // 충돌체 컴포넌트
	CCollider2D*		m_FeetCollider; // 땅 체크용 충돌체 컴포넌트

	Ptr<StateMachine>	m_StateMachine; // 상태 머신

	bool 			    m_Land; // 땅에 닿아있는지 여부
	bool                m_Jump; // 점프 중인지 여부
	bool 			    m_DoubleJump; // 점프 중인지 여부

public:
	void SetTarget(Ptr<GameObject> _Target) { m_Target = _Target; }
	void SetBodyCollider(CCollider2D* _Collider) { m_BodyCollider = _Collider; }
	void SetFeetCollider(CCollider2D* _Collider) { m_FeetCollider = _Collider; }
	void SetStateMachine(Ptr<StateMachine> _StateMachine) { m_StateMachine = _StateMachine; }

public:
	void Begin() override;
    virtual void Tick() override;

private:
    void Move();
    void Jump();
    void Slide();

public:
    virtual void Skill();

	CLONE(CPlayerScript);

public:
    CPlayerScript();
    virtual ~CPlayerScript();
};

