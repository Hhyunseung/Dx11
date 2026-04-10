#pragma once
#include "CScript.h"

enum class ESkillBGFadeState
{
    FadeIn,
    Active,
    FadeOut
};

class CSkillBGScript :
    public CScript
{
private:
	float m_FadeInDuration; // 페이드 인 지속 시간
	float m_FadeOutDuration; // 페이드 아웃 지속 시간
	float m_Timer;
	ESkillBGFadeState m_State;
    
private:
    void SetAlpha(float _Alpha);

public:
    void BeginFadeOut();

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    // 저장 불러오기
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CSkillBGScript);

public:
    CSkillBGScript();
    virtual ~CSkillBGScript();
};

