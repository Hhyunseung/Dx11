#pragma once
#include "CUIButtonScript.h"
class CJumpButtonScript :
    public CUIButtonScript
{
public:
	virtual void Tick() override;

	virtual void OnButtonDown();

public:
	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CJumpButtonScript);

public:
	CJumpButtonScript();
	virtual ~CJumpButtonScript();
};

