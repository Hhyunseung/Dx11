#include "pch.h"
#include "CFlipbookClickScript.h"

#include "GameObject.h"
#include "CFlipbookRender.h"

CFlipbookClickScript::CFlipbookClickScript()
	: CUIButtonScript(SCRIPT_TYPE::FLIPBOOKCLICKSCRIPT)
	, m_FlipbookIdx(1)
	, m_DefaultFlipbookIdx(0)
	, m_FPS(8.f)
	, m_WaitingForFinish(false)
{

}

CFlipbookClickScript::~CFlipbookClickScript()
{
}

void CFlipbookClickScript::Begin()
{
	Ptr<CFlipbookRender> pFlipbookRender = GetOwner()->FlipbookRender();
	pFlipbookRender->Play(m_DefaultFlipbookIdx, m_FPS, -1);
}

void CFlipbookClickScript::Tick()
{
	CUIButtonScript::Tick();

	Ptr<CFlipbookRender> pFlipbookRender = GetOwner()->FlipbookRender();

	if (pFlipbookRender == nullptr)
		return;

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
