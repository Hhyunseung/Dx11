#pragma once
#include "Entity.h"

#include "GameObject.h"

class Layer :
    public Entity
{
private:
    vector<Ptr<GameObject>>     m_vecParents;   // Layer 에 소속된 최상위 부모타입 오브젝트들
    vector<Ptr<GameObject>>     m_vecAllObjects;// Layer 에 소속된 모든(부모, 자식타입) 오브젝트들
    int     	                m_LayerIdx;     // Layer 본인의 인덱스(0 ~ 31)

public:
    void AddObject(Ptr<GameObject> _Obj);
    void RegisterObject(Ptr<GameObject> _Obj) { m_vecAllObjects.push_back(_Obj); }
    void DeregisterObject() { m_vecAllObjects.clear(); }

    /// 배열이 복사되어 들어감
    // vector<Ptr<GameObject>> GetParentObjects() { return m_vecParents; }

    /// 원본 유지 // 수정하지 않기 위해 const 
    const vector<Ptr<GameObject>>& GetParentObjects() { return m_vecParents; } /// 처음 Init 할 때 넣어줌
    const vector<Ptr<GameObject>>& GetAllObjects() { return m_vecAllObjects; } /// 한번이라도 프레임이 지나야 넣어줌

public:
    void Begin();
    void Tick();
    void FinalTick();
    void Render();

public:
    Layer();
	virtual ~Layer();

    friend class ALevel;
};

