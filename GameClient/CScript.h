#pragma once
#include "Component.h"

#include "Source/ScriptMgr.h"

class CCollider2D;

class CScript :
    public Component
{
private:
    int     m_ScriptType;

public:
    int GetScriptType() { return m_ScriptType; }

public:
    virtual void Tick() = 0;
	virtual void FinalTick() final {}; // final : 자식 클래스에서 재정의 금지
    
public:
    /// 역참조 문제로 Ptr 이 아니라 그냥 포인터 사용
    /// 가상 함수의 주소를 넘기면 안됨
    /// 원하는 스크립트에 구현하는걸로
    //virtual void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider) {}
    //virtual void Overlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider) {}
    //virtual void EndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider) {}

protected:
    void Destroy();

public:
    virtual CScript* Clone() = 0;

public:
    CScript(int _ScriptType);
    virtual ~CScript();
};


#define ADD_DYNAMIC_BEGIN_OVERLAP(MemFunc) Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&MemFunc);
#define ADD_DYNAMIC_OVERLAP(MemFunc) Collider2D()->AddDynamicOverlap(this, (COLLISION_EVENT)&MemFunc);
#define ADD_DYNAMIC_END_OVERLAP(MemFunc) Collider2D()->AddDynamicEndOverlap(this, (COLLISION_EVENT)&MemFunc);
