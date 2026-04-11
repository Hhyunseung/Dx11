#include "pch.h"
#include "CGamePlayUIScript.h"

#include "GamePlayMgr.h"
#include "KeyMgr.h"

#include "GameObject.h"

#include "CUIButtonScript.h"
#include "CJumpButtonScript.h"
#include "CSlideButtonScript.h"
#include "CHPBarScript.h"
#include "CHitEffectScript.h"

#include "CPausePanelScript.h"
#include "CPauseButtonScript.h"
#include "CResumeButtonScript.h"
#include "CRestartButtonScript.h"
#include "CQuitButtonScript.h"

CGamePlayUIScript::CGamePlayUIScript()
	: CScript(SCRIPT_TYPE::GAMEPLAYUISCRIPT)
	, m_JumpBtn(nullptr)
	, m_SlideBtn(nullptr)
	, m_JumpRequest(false)
	, m_SlideRequest(false)
	, m_HPBar(nullptr)
	, m_HitEffect(nullptr)
	, m_PauseBtn(nullptr)
	, m_PausePanel(nullptr)
	, m_ResumeBtn(nullptr)	
	, m_RestartBtn(nullptr)
	, m_QuitBtn(nullptr)
{
}

CGamePlayUIScript::~CGamePlayUIScript()
{

}

void CGamePlayUIScript::Begin()
{
	GamePlayMgr::GetInst()->SetGamePlayUIScript(this);

	GameObject* pHitEffectObj = GetOwner()->FindChildByScript<CHitEffectScript>();
	if (pHitEffectObj != nullptr)
	{
		m_HitEffect = pHitEffectObj->GetScript<CHitEffectScript>().Get();
	}


	GameObject* pJumpObj = GetOwner()->FindChildByScript<CJumpButtonScript>();
	if (pJumpObj != nullptr)
	{
		m_JumpBtn = pJumpObj->GetScript<CJumpButtonScript>().Get();
	}

	GameObject* pSlideObj = GetOwner()->FindChildByScript<CSlideButtonScript>();
	if (pSlideObj != nullptr)
	{
		m_SlideBtn = pSlideObj->GetScript<CSlideButtonScript>().Get();
	}

	GameObject* pHPBarObj = GetOwner()->FindChildByScript<CHPBarScript>();
	if (pHPBarObj != nullptr)
	{
		m_HPBar = pHPBarObj->GetScript<CHPBarScript>().Get();
	}

	GameObject* pPauseObj = GetOwner()->FindChildByScript<CPauseButtonScript>();
	if (pPauseObj != nullptr)
	{
		m_PauseBtn = pPauseObj->GetScript<CPauseButtonScript>().Get();
	}

	m_PausePanel = GetOwner()->FindChildByScript<CPausePanelScript>();

	if (m_PausePanel != nullptr)
	{
		GameObject* pResumeObj = m_PausePanel->FindChildByScript<CResumeButtonScript>();
		if (pResumeObj != nullptr)
			m_ResumeBtn = pResumeObj->GetScript<CResumeButtonScript>().Get();

		GameObject* pRestartObj = m_PausePanel->FindChildByScript<CRestartButtonScript>();
		if (pRestartObj != nullptr)
			m_RestartBtn = pRestartObj->GetScript<CRestartButtonScript>().Get();

		GameObject* pQuitObj = m_PausePanel->FindChildByScript<CQuitButtonScript>();
		if (pQuitObj != nullptr)
			m_QuitBtn = pQuitObj->GetScript<CQuitButtonScript>().Get();
	}
}

void CGamePlayUIScript::Tick()
{
	if (KEY_TAP(KEY::ESC))
	{
		GamePlayMgr::GetInst()->TogglePause();
	}

	bool isPaused = GamePlayMgr::GetInst()->IsPaused();

	if (m_PausePanel != nullptr)
	{
		m_PausePanel->SetActive(isPaused);
	}
}

