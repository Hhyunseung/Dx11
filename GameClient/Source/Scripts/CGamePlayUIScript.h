#pragma once
#include "CScript.h"

class CJumpButtonScript;
class CSlideButtonScript;

class CGamePlayUIScript :
    public CScript
{
private:
	CJumpButtonScript*		m_JumpBtn;
	CSlideButtonScript*		m_SlideBtn;

	bool m_JumpRequest;
	bool m_SlideRequest;

public:
	CJumpButtonScript* GetJumpButton() const { return m_JumpBtn; }
	void SetJumpButton(CJumpButtonScript* _Btn) { m_JumpBtn = _Btn; }
	CSlideButtonScript* GetSlideButton() const { return m_SlideBtn; }
	void SetSlideButton(CSlideButtonScript* _Btn) { m_SlideBtn = _Btn; }

public:
	virtual void Begin() override;
	virtual void Tick() override;

	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CGamePlayUIScript);

public:
	CGamePlayUIScript();
	virtual ~CGamePlayUIScript();
};

