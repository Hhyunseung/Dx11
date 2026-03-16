#pragma once
#include "CScript.h"
class CScrollScript :
    public CScript
{
private:
    float m_fSpeed;

public:
	void SetSpeed(float _Speed) { m_fSpeed = _Speed; }

public:
    void Tick();

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CScrollScript);

public:
    CScrollScript();
    virtual ~CScrollScript();
};

