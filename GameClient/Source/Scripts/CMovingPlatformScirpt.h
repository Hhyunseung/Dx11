#pragma once
#include "CScript.h"

class CMovingPlatformScirpt :
    public CScript
{
private:
    Vec3 m_StartPos;
    Vec3 m_EndPos;
    float m_Speed;
	bool m_IsMovingToEnd;


private:
    void Move();

public:
    void Tick();

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CMovingPlatformScirpt);

public:
    CMovingPlatformScirpt();
    virtual ~CMovingPlatformScirpt();
};

