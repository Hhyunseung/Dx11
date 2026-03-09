#pragma once
#include "CScript.h"
class CEnemy :
    public CScript
{
private:
	float m_Speed;

public:
    void Tick();

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CEnemy);

public:
    CEnemy();
    CEnemy(float Speed);
    virtual ~CEnemy();
};

