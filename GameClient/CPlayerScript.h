#pragma once
#include "CScript.h"

#include "CMissileScript.h"
#include "StateMachine.h"

class CPlayerScript;

class PlayerState 
	: public Entity
{
private:
	CPlayerScript*      m_Owner;
	PLAYER_STATE_ID       m_Id;

public:
	CPlayerScript* GetOwner() { return m_Owner; }
	PLAYER_STATE_ID GetId() { return m_Id; }

public:
	virtual void Enter(PLAYER_STATE_ID _Prev) = 0;
	virtual void Tick(float dt) = 0;
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

	StateMachine		m_StateMachine; // 상태 머신

	bool 			    m_Land; // 땅에 닿아있는지 여부
	bool                m_Jump; // 점프 중인지 여부
	bool 			    m_DoubleJump; // 점프 중인지 여부

public:
	void SetTarget(Ptr<GameObject> _Target) { m_Target = _Target; }

public:
	void Begin() override;
    virtual void Tick() override;

private:
    void Move();
    void Jump();
    void Slide();

public:
    virtual void Skill();

public:
    CPlayerScript();
    virtual ~CPlayerScript();
};

