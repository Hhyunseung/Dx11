#pragma once
#include "CScript.h"

class CJellyScript :
    public CScript
{
private:
    int     m_Score;
    bool    m_Magnet;
    float   m_MagnetRange;
    float   m_MagnetSpeed;

public:
	void BeginOverlap(CCollider2D* _This, CCollider2D* _Other);

public:
    virtual void Begin() override;
    virtual void Tick() override;

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CJellyScript);

public:
    CJellyScript();
    virtual ~CJellyScript();
};