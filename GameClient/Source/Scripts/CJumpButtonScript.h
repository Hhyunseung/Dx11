#pragma once
#include "CUIButtonScript.h"
class CJumpButtonScript :
    public CUIButtonScript
{
private:
	bool m_Clicked; // 버튼이 클릭된 상태인지 여부

public:
	bool ConsumeClick();

public:
	virtual void Tick() override;

	virtual void OnButtonDown();
	virtual void OnButtonUp();

public:
	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CJumpButtonScript);

public:
	CJumpButtonScript();
	virtual ~CJumpButtonScript();
};

