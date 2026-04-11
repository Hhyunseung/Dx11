#pragma once
#include "CScript.h"

class CJumpButtonScript;
class CSlideButtonScript;
class CHPBarScript;
class CHitEffectScript;

class CUIButtonScript;

class CPausePanelScript;
class CPauseButtonScript;
class CResumeButtonScript;
class CRestartButtonScript;
class CQuitButtonScript;


class CGamePlayUIScript :
    public CScript
{
private:
	CJumpButtonScript*		m_JumpBtn;
	CSlideButtonScript*		m_SlideBtn;

	CHPBarScript*			m_HPBar;

	CHitEffectScript*		m_HitEffect;

	CUIButtonScript*		m_PauseBtn;


	GameObject*				m_PausePanel;
	CResumeButtonScript*	m_ResumeBtn;
	CRestartButtonScript*	m_RestartBtn;
	CQuitButtonScript*		m_QuitBtn;

	bool m_JumpRequest;
	bool m_SlideRequest;

public:
	CJumpButtonScript* GetJumpButton() const { return m_JumpBtn; }
	void SetJumpButton(CJumpButtonScript* _Btn) { m_JumpBtn = _Btn; }
	CSlideButtonScript* GetSlideButton() const { return m_SlideBtn; }
	void SetSlideButton(CSlideButtonScript* _Btn) { m_SlideBtn = _Btn; }

	CHPBarScript* GetHPBar() const { return m_HPBar; }
	void SetHPBar(CHPBarScript* _HPBar) { m_HPBar = _HPBar; }

	CHitEffectScript* GetHitEffect() const { return m_HitEffect; }

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

