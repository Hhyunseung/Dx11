#pragma once

#include "CScript.h"

class CMissileScript :
    public CScript
{
private:
    Ptr<GameObject> m_Target;

public:
	void SetTarget(Ptr<GameObject> _Target) { m_Target = _Target; }

private:
	float m_Speed;
	float m_Time;

public:
	virtual void Begin();
    virtual void Tick() override;
	void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
	void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
	void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

	CLONE(CMissileScript);

public:
    CMissileScript();
    virtual ~CMissileScript();
};

