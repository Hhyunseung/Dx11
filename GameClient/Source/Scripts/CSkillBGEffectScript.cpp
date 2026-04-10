#include "pch.h"
#include "CSkillBGEffectScript.h"

#include "GameObject.h"
#include "CFlipbookRender.h"
#include "GamePlayMgr.h"

CSkillBGEffectScript::CSkillBGEffectScript()
	: CScript(SCRIPT_TYPE::SKILLBGEFFECTSCRIPT)
	, m_FPS(12.f)
{
}

CSkillBGEffectScript::~CSkillBGEffectScript()
{
}

void CSkillBGEffectScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FPS, L"FPS", true, 0.f);
}

void CSkillBGEffectScript::Begin()
{
	Ptr<CFlipbookRender> pRender = GetOwner()->FlipbookRender();
	if (pRender != nullptr)
		pRender->Play(0, m_FPS, 0);
}

void CSkillBGEffectScript::Tick()
{
	if (GamePlayMgr::GetInst()->IsPaused())
		return;

	Ptr<CFlipbookRender> pRender = GetOwner()->FlipbookRender();
	if (pRender != nullptr && pRender->IsAnimationComplete())
		Destroy();
}

void CSkillBGEffectScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_FPS, sizeof(float), 1, _File);
}

void CSkillBGEffectScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_FPS, sizeof(float), 1, _File);
}
