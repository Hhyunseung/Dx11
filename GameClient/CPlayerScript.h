#pragma once
#include "CScript.h"

#include "CMissileScript.h"

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
    virtual void Tick() override;

private:
    void Move();
    void Shoot();
    void Jump();

public:
    CPlayerScript();
    virtual ~CPlayerScript();
};

