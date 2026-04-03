#pragma once
#include "CUIButtonScript.h"
class CSlideButtonScript :
    public CUIButtonScript
{
private:
	bool m_MouseHolding; // 마우스 버튼이 눌린 상태인지 여부

public:
	virtual void Tick() override;
	virtual void OnButtonDown();
	virtual void OnButtonUp();

public:
	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CSlideButtonScript);

public:
	CSlideButtonScript();
	virtual ~CSlideButtonScript();
};

