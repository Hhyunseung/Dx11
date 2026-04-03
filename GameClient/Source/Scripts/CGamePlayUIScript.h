#pragma once
#include "CScript.h"

class CJumpButtonScript;
class CSlideButtonScript;

class CGamePlayUIScript :
    public CScript
{
private:
	Ptr<CJumpButtonScript>		m_JumpBtn;
	Ptr<CSlideButtonScript>		m_SlideBtn;

	bool m_JumpRequest;
	bool m_SlideRequest;

public:
	void SetJumpButton(Ptr<CJumpButtonScript> _Btn) { m_JumpBtn = _Btn; }
	void SetSlideButton(Ptr<CSlideButtonScript> _Btn) { m_SlideBtn = _Btn; }

public:
	bool ConsumeJumpReqeust();
	bool IsSlideHeld() const { return m_SlideRequest; }

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

