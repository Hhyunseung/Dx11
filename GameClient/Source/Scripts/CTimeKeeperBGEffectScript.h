#pragma once
#include "CScript.h"

enum class ETKBGEffectState
{
	Intro,
	Loop,
};

class CTimeKeeperBGEffectScript : public CScript
{
private:
	float			 m_FPS;
	ETKBGEffectState m_State;

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CTimeKeeperBGEffectScript);

public:
	CTimeKeeperBGEffectScript();
	virtual ~CTimeKeeperBGEffectScript();
};
