#pragma once
#include "CScript.h"

class CScoreUIScript :
    public CScript
{
private:
	wstring m_FontText;
	float PosX;
	float PosY;

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CScoreUIScript);

public:
	CScoreUIScript();
	virtual ~CScoreUIScript();
};

