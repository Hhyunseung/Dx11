#pragma once
#include "CScript.h"

#include "CMissileScript.h"

class CPlayerScript;

class PlayerState 
	: public Entity
{
private:
	CPlayerScript*      m_Owner;
	PLAYERSTATEID       m_Id;

public:
	CPlayerScript* GetOwner() { return m_Owner; }

public:
	virtual void Enter(PLAYERSTATEID _Prev) = 0;
	virtual void Tick(float dt) = 0;
	virtual void Exit(PLAYERSTATEID _Next) = 0;

public:
	PlayerState(CPlayerScript* _Owner, PLAYERSTATEID _Id) 
		: m_Owner(_Owner), m_Id(_Id) {}
	virtual ~PlayerState() {}
};





class CPlayerScript :
    public CScript
{
private:
    Ptr<GameObject>     m_Target;

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

