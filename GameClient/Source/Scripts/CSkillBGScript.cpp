#include "pch.h"
#include "CSkillBGScript.h"

#include "GamePlayMgr.h"
#include "TimeMgr.h"
#include "GameObject.h"

CSkillBGScript::CSkillBGScript()
	: CScript(SCRIPT_TYPE::SKILLBGSCRIPT)
	, m_FadeInDuration(0.5f)
	, m_FadeOutDuration(0.5f)
	, m_Timer(0.f)
	, m_State(ESkillBGFadeState::FadeIn)
{
}

CSkillBGScript::~CSkillBGScript()
{
}

void CSkillBGScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FadeInDuration, L"FadeInDuration", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FadeOutDuration, L"FadeOutDuration", true, 0.f);
}

void CSkillBGScript::Begin()
{
	if (GetOwner()->MeshRender() != nullptr)
		GetOwner()->MeshRender()->CreateDynamicMaterial();

	m_State = ESkillBGFadeState::FadeIn;
	m_Timer = 0.f;
	SetAlpha(0.f);
}

void CSkillBGScript::Tick()
{
	if (GamePlayMgr::GetInst()->IsPaused())
		return;

	m_Timer += DT;

	switch (m_State)
	{
	case ESkillBGFadeState::FadeIn:
	{
  		float alpha = (m_FadeInDuration > 0.f) ? (m_Timer / m_FadeInDuration) : 1.f;
		if (alpha >= 1.f)
		{
			alpha = 1.f;
			m_State = ESkillBGFadeState::Active;
			m_Timer = 0.f;
		}
		SetAlpha(alpha);
	}
	break;
	case ESkillBGFadeState::FadeOut:
	{
		float alpha = 1.f - ((m_FadeOutDuration > 0.f) ? (m_Timer / m_FadeOutDuration) : 1.f);
		if (alpha <= 0.f)
		{
			SetAlpha(0.f);
			Destroy();
			return;
		}
		SetAlpha(alpha);
	}
	break;
	}
}

void CSkillBGScript::BeginFadeOut()
{
	if (m_State == ESkillBGFadeState::FadeOut)
		return;

	m_State = ESkillBGFadeState::FadeOut;
	m_Timer = 0.f;
}

void CSkillBGScript::SetAlpha(float _Alpha)
{
	if (GetOwner()->MeshRender() != nullptr && GetOwner()->MeshRender()->GetMaterial() != nullptr)
		GetOwner()->MeshRender()->GetMaterial()->SetScalar(SCALAR_PARAM::FLOAT_0, _Alpha);
}

void CSkillBGScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_FadeInDuration, sizeof(float), 1, _File);
	fwrite(&m_FadeOutDuration, sizeof(float), 1, _File);
}

void CSkillBGScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_FadeInDuration, sizeof(float), 1, _File);
	fread(&m_FadeOutDuration, sizeof(float), 1, _File);
}

