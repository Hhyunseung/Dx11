#include "pch.h"
#include "CGamePlayUIScript.h"

#include "GamePlayMgr.h"
#include "KeyMgr.h"

#include "CJumpButtonScript.h"
#include "CSlideButtonScript.h"

CGamePlayUIScript::CGamePlayUIScript()
	: CScript(SCRIPT_TYPE::GAMEPLAYUISCRIPT)
	, m_JumpBtn(nullptr)
	, m_SlideBtn(nullptr)
	, m_JumpRequest(false)
	, m_SlideRequest(false)
{
}

CGamePlayUIScript::~CGamePlayUIScript()
{
}


void CGamePlayUIScript::Begin()
{
	GamePlayMgr::GetInst()->SetGamePlayUIScript(this);
}

void CGamePlayUIScript::Tick()
{
	// 점프
	if (KEY_TAP(KEY::SPACE))
	{
		m_JumpRequest = true;
	}

	if (m_JumpBtn != nullptr && m_JumpBtn->GetPressed())
	{
		m_JumpRequest = true;
	}

	// 슬라이드
	bool bKeySlide = KEY_PRESSED(KEY::DOWN);
	bool bUISlide = (m_SlideBtn != nullptr && m_SlideBtn->GetPressed());

	m_SlideRequest = (bKeySlide || bUISlide);


	// 점프가 들어온 프레임에는 슬라이드 강제 해제하고 싶다면
	if (m_JumpRequest)
	{
		m_SlideRequest = false;
		m_SlideBtn->SetPressed(false);
	}
}

bool CGamePlayUIScript::ConsumeJumpReqeust()
{
	if (!m_JumpRequest)
		return false;

	m_JumpRequest = false;
	return true;
}
