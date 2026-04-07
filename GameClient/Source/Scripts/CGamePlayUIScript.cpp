#include "pch.h"
#include "CGamePlayUIScript.h"

#include "GamePlayMgr.h"
#include "KeyMgr.h"

#include "GameObject.h"

#include "CJumpButtonScript.h"
#include "CSlideButtonScript.h"
#include "CHPBarScript.h"

CGamePlayUIScript::CGamePlayUIScript()
	: CScript(SCRIPT_TYPE::GAMEPLAYUISCRIPT)
	, m_JumpBtn(nullptr)
	, m_SlideBtn(nullptr)
	, m_JumpRequest(false)
	, m_SlideRequest(false)
	, m_HPBar(nullptr)
{
}

CGamePlayUIScript::~CGamePlayUIScript()
{

}

void CGamePlayUIScript::Begin()
{
	GamePlayMgr::GetInst()->SetGamePlayUIScript(this);


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
	if (pSlideObj != nullptr)
	{
		m_HPBar = pHPBarObj->GetScript<CHPBarScript>().Get();
	}
}

void CGamePlayUIScript::Tick()
{

}

