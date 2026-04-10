#pragma once
#include "CScript.h"

class CSkillBGEffectScript : public CScript
{
private:
	float m_FPS;

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CSkillBGEffectScript);

public:
	CSkillBGEffectScript();
	virtual ~CSkillBGEffectScript();
};
