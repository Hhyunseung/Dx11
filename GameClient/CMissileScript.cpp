#include "pch.h"
#include "CMissileScript.h"
#include "CCollider2D.h"
#include "GameObject.h"
#include "LevelMgr.h"
#include "TimeMgr.h"

CMissileScript::CMissileScript()
	: m_Speed(200.f)
	, m_Time(0.f)
{

}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::Begin()
{
	Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CMissileScript::BeginOverlap);
	Collider2D()->AddDynamicOverlap(this, (COLLISION_EVENT)&CMissileScript::Overlap);
	Collider2D()->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CMissileScript::EndOverlap);
}

void CMissileScript::Tick()
{
	// 추적 대상이 있다면 
	if (IsValid(m_Target))
	{
		Vec3 vTargetPos = m_Target->Transform()->GetWorldPos();
		Vec3 vPos = Transform()->GetRelativePos();

		// 타겟까지의 방향벡터
		Vec3 vDirToTarget = vTargetPos - vPos;
		vDirToTarget.Normalize();

		vPos += vDirToTarget * m_Speed * DT;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}

}

void CMissileScript::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{

}

void CMissileScript::Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CMissileScript::EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}
