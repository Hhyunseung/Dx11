#pragma once
#include "CScript.h"

#include "CMissileScript.h"
#include "CStateMachine.h"


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
	PlayerState(int _ScriptType, CPlayerScript* _Owner, PLAYER_STATE_ID _Id) 
		: CScript(_ScriptType), m_Owner(_Owner), m_Id(_Id) {}
	virtual ~PlayerState() {}
};





class CPlayerScript :
    public CScript
{
private:
    Ptr<GameObject>		m_Target;

	CCollider2D*		m_BodyCollider; // 충돌체 컴포넌트
	CCollider2D*		m_FeetCollider; // 땅 체크용 충돌체 컴포넌트

	Ptr<CStateMachine>	m_StateMachine; // 상태 머신

	float				m_PrevFeetY; // 땅 체크용 충돌체의 이전 y 좌표
	float			    m_CurFeetY; // 땅 체크용 충돌체의 y 좌표

	float			    m_gravity; // 중력 가속도

	float 			    m_JumpPower; // 점프 힘
	float			    m_DoubleJumpPower; // 더블 점프 힘

	bool 			    m_IsLand; // 땅에 닿아있는지 여부
	bool                m_IsJump; // 점프 중인지 여부
	bool 			    m_IsDoubleJump; // 점프 중인지 여부

public:
	void SetTarget(Ptr<GameObject> _Target) { m_Target = _Target; }
	void SetBodyCollider(CCollider2D* _Collider) { m_BodyCollider = _Collider; }
	void SetFeetCollider(CCollider2D* _Collider) { m_FeetCollider = _Collider; }
	void SetStateMachine(Ptr<CStateMachine> _StateMachine) { m_StateMachine = _StateMachine; }

	void ChangeState(PLAYER_STATE_ID _NextId) { m_StateMachine->ChangeState(_NextId); }

	void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

	void FeetBeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
	void FeetOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
	void FeetEndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

public:
	GET_SET(bool, IsLand);
	GET_SET(bool, IsJump);
	GET_SET(bool, IsDoubleJump);

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

