#pragma once
#include "CScript.h"

class CMovingPlatformScirpt :
    public CScript
{
private:
    Vec3 m_StartPos;
    Vec3 m_EndPos;
    Vec3 m_PrevPos;
    Vec3 m_FrameDelta;

    float m_EndPosX;
	float m_EndPosY;

    float m_Speed;
	bool m_IsMovingToEnd;


    EObjectID   m_ObjectID;     // 풀링용 ID


public:
    Vec3 GetFrameDelta() const { return m_FrameDelta; }
    void SetFrameDelta(Vec3 _FrameDelta) { m_FrameDelta = _FrameDelta; }

public:
	virtual void Init() override;
    virtual void OnSpawn() override;
    virtual void ApplySpawnInfo(const FSpawnInfo& info) override;
    virtual void Begin() override;
    virtual void Tick() override;

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CMovingPlatformScirpt);

public:
    CMovingPlatformScirpt();
    virtual ~CMovingPlatformScirpt();
};

