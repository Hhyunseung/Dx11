#pragma once
#include "Component.h"

#include "Source/ScriptMgr.h"

// forward
struct FSpawnInfo;

enum class SCRIPT_PARAM
{
    INT,
    FLOAT,
    BOOL,
    VEC2,
    VEC4,
    MATRIX,

    TEXTURE,
    MATERIAL,
    PREFAB,

    EObjectID,
};

struct tScriptParam
{
    SCRIPT_PARAM    Param;
    void*           Data;
    wstring         Desc;
    bool            IsInput;
    float           Step;
};


class CCollider2D;

class CScript :
    public Component
{
private:
    int                  m_ScriptType;
    vector<tScriptParam> m_vecScriptParam; // 에디터에 노출할 변수 데이터 저장

public:
    int GetScriptType() { return m_ScriptType; }

protected:
    void AddScriptParam(SCRIPT_PARAM _Type, void* _Data, const wstring& _Desc, bool _IsInput = true, float _Step = 1.f) { m_vecScriptParam.push_back(tScriptParam{ _Type , _Data, _Desc, _IsInput, _Step }); }
    void Instantiate(class APrefab* _Prefab, int _LayerIdx, Vec3 _WorldPos);

public:
    const vector<tScriptParam>& GetScriptParam() { return m_vecScriptParam; }

public:
    virtual void Tick() = 0;
    virtual void FinalTick() final {}; // final : 자식 클래스에서 재정의 금지

    // Called when object is spawned / taken from pool
    virtual void OnSpawn() {}

    // Apply spawn-time overrides from FSpawnInfo. Called when object is instantiated/spawned.
    virtual void ApplySpawnInfo(const FSpawnInfo& info) {}

public:


protected:
    void Destroy();

public:
    virtual CScript* Clone() = 0;

public:
    CScript(int _ScriptType);
	CScript(const CScript& _Origin);
    virtual ~CScript();
};


#define ADD_DYNAMIC_BEGIN_OVERLAP(MemFunc) Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&MemFunc);
#define ADD_DYNAMIC_OVERLAP(MemFunc) Collider2D()->AddDynamicOverlap(this, (COLLISION_EVENT)&MemFunc);
#define ADD_DYNAMIC_END_OVERLAP(MemFunc) Collider2D()->AddDynamicEndOverlap(this, (COLLISION_EVENT)&MemFunc);
