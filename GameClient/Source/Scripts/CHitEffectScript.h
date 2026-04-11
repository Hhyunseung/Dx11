#pragma once
#include "CScript.h"

class CHitEffectScript : public CScript
{
private:
	float m_Alpha;
	float m_FadeSpeed;

public:
	void Trigger();

private:
	void SetAlpha(float _Alpha);

public:
	virtual void Begin() override;
	virtual void Tick()  override;

	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CHitEffectScript);

public:
	CHitEffectScript();
	virtual ~CHitEffectScript();
};
