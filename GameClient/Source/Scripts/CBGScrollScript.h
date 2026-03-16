#pragma once
#include "CScript.h"

class CBGScrollScript :
    public CScript
{
private:
    float   m_SpeedX;
    float   m_SpeedY;
    Vec2    m_UV;

public:
	GET_SET(float, SpeedX);
	GET_SET(float, SpeedY);

public:
    virtual void Init() override;
    void Tick();

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBGScrollScript);

public:
    CBGScrollScript();
    virtual ~CBGScrollScript();
};

