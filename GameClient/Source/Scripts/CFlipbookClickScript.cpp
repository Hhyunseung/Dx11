#include "pch.h"
#include "CFlipbookClickScript.h"

#include "GameObject.h"
#include "CFlipbookRender.h"
#include "GamePlayMgr.h"

CFlipbookClickScript::CFlipbookClickScript()
	: CUIButtonScript(SCRIPT_TYPE::FLIPBOOKCLICKSCRIPT)
	, m_FlipbookIdx(1)
	, m_DefaultFlipbookIdx(0)
	, m_FPS(8.f)
	, m_WaitingForFinish(false)
	, m_CharType(ECharacterType::TimeKeeper)
{
}

CFlipbookClickScript::~CFlipbookClickScript()
{
}

void CFlipbookClickScript::Begin()
{
	m_CharType = GamePlayMgr::GetInst()->GetSelectedCharacterType();
	m_DefaultFlipbookIdx = (int)m_CharType * 2;
	m_FlipbookIdx        = (int)m_CharType * 2 + 1;

	Ptr<CFlipbookRender> pFlipbookRender = GetOwner()->FlipbookRender();
	pFlipbookRender->Play(m_DefaultFlipbookIdx, m_FPS, -1);
}

void CFlipbookClickScript::Tick()
{
	CUIButtonScript::Tick();

	Ptr<CFlipbookRender> pFlipbookRender = GetOwner()->FlipbookRender();

	if (pFlipbookRender == nullptr)
		return;

	ECharacterType curType = GamePlayMgr::GetInst()->GetSelectedCharacterType();
	if (curType != m_CharType)
	{
		m_CharType           = curType;
		m_DefaultFlipbookIdx = (int)m_CharType * 2;
		m_FlipbookIdx        = (int)m_CharType * 2 + 1;
		m_WaitingForFinish   = false;
	}

	if (m_WaitingForFinish)
	{
		if (pFlipbookRender->IsAnimationComplete())
		{
			m_WaitingForFinish = false;
		}
	}
	else
	{
		if (pFlipbookRender->GetCurFlipbook() != m_DefaultFlipbookIdx)
		{
			pFlipbookRender->Play(m_DefaultFlipbookIdx, m_FPS, -1);
		}
	}
}

void CFlipbookClickScript::OnButtonClick()
{
	Ptr<CFlipbookRender> pFlipbookRender = GetOwner()->FlipbookRender();

	if (pFlipbookRender == nullptr)
		return;

	pFlipbookRender->Play(m_FlipbookIdx, m_FPS, 0);
	m_WaitingForFinish = true;
}
