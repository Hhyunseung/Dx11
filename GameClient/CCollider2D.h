#pragma once
#include "Component.h"

#include "CScript.h"

// 반환타입이 void 면서
// CScript(혹은 파생된)의 멤버함수이면서 
// 매개변수로 (CCollider2D*, CCollider2D*)를 받는 함수
class CCollider2D;
typedef void (CScript::* COLLISION_EVENT)(CCollider2D*, CCollider2D*);

struct COLLISION_DELEGATE
{
    class CScript*  Inst;
    COLLISION_EVENT MemFunc; 
    /// 컴파일 시점엔 해당 멤버함수가 가져아할 포인터 크기를 모른다 CScript 는 전방선언이니까
    /// 그래서 기본 8바이트를 담아둔다
    /// 똑같은 자료형을 상황에 따라 다른 사이즈로 잡아주니까 메모리가 꼬인것
    /// 이런 상황에서는 전방선언이 아닌 #include 로 헤더파일을 해주어야..
    /// ex CCollider2D::BeginOverlap 에서는 크기를 알 수 있어서 24바이트가 나온다
};

class CCollider2D :
    public Component
{
private:
    Vec2    m_Offset;
    Vec2    m_Scale;

    int     m_OverlapCount;
    Matrix  m_matWorld;

    vector<COLLISION_DELEGATE> m_vecBeginDel;
    vector<COLLISION_DELEGATE> m_vecOverDel;
    vector<COLLISION_DELEGATE> m_vecEndDel;

public:
    void AddDynamicBeginOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc);
    void AddDynamicOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc);
    void AddDynamicEndOverlap(CScript* _Inst, COLLISION_EVENT _MemFunc);


public:
    GET_SET(Vec2, Offset);
    GET_SET(Vec2, Scale);

    const Matrix& GetWorldMat() const { return m_matWorld; }

private:
    void BeginOverlap(Ptr<CCollider2D> _Other);
    void Overlap(Ptr<CCollider2D> _Other);
    void EndOverlap(Ptr<CCollider2D> _Other);

public:
	virtual void FinalTick() override;

public:
    CCollider2D();
    virtual ~CCollider2D();

    friend class CollisionMgr;
};

