#include "pch.h"
#include "CTimeKeeperBGEffectScript.h"

#include "GameObject.h"
#include "CFlipbookRender.h"
#include "GamePlayMgr.h"

CTimeKeeperBGEffectScript::CTimeKeeperBGEffectScript()
	: CScript(SCRIPT_TYPE::TIMEKEEPERBGEFFECTSCRIPT)
	, m_FPS(12.f)
	, m_State(ETKBGEffectState::Intro)
{
}

CTimeKeeperBGEffectScript::~CTimeKeeperBGEffectScript()
{
}

void CTimeKeeperBGEffectScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FPS, L"FPS", true, 0.f);
}

void CTimeKeeperBGEffectScript::Begin()
{
	m_State = ETKBGEffectState::Intro;

	Ptr<CFlipbookRender> pRender = GetOwner()->FlipbookRender();
	if (pRender != nullptr)
		pRender->Play(0, m_FPS, 0);
}

void CTimeKeeperBGEffectScript::Tick()
{
	if (GamePlayMgr::GetInst()->IsPaused())
		return;

	if (m_State != ETKBGEffectState::Intro)
		return;

	Ptr<CFlipbookRender> pRender = GetOwner()->FlipbookRender();
	if (pRender != nullptr && pRender->IsAnimationComplete())
	{
		m_State = ETKBGEffectState::Loop;
		pRender->Play(1, m_FPS, -1);
	}
}

void CTimeKeeperBGEffectScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_FPS, sizeof(float), 1, _File);
}

void CTimeKeeperBGEffectScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_FPS, sizeof(float), 1, _File);
}
