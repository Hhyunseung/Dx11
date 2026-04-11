#include "pch.h"
#include "CHitEffectScript.h"

#include "GameObject.h"
#include "TimeMgr.h"
#include "GamePlayMgr.h"

CHitEffectScript::CHitEffectScript()
	: CScript(SCRIPT_TYPE::HITEFFECTSCRIPT)
	, m_Alpha(0.f)
	, m_FadeSpeed(2.f)
{
}

CHitEffectScript::~CHitEffectScript()
{
}

void CHitEffectScript::Begin()
{
	if (GetOwner()->MeshRender() != nullptr)
	{
		GetOwner()->MeshRender()->CreateDynamicMaterial();
		SetAlpha(0.f);
	}
}

void CHitEffectScript::Trigger()
{
	m_Alpha = 1.0f;
	SetAlpha(m_Alpha);
}

void CHitEffectScript::Tick()
{
	if (GamePlayMgr::GetInst()->IsPaused())
		return;

	if (m_Alpha <= 0.f)
		return;

	m_Alpha -= m_FadeSpeed * DT;
	if (m_Alpha < 0.f)
		m_Alpha = 0.f;

	SetAlpha(m_Alpha);
}

void CHitEffectScript::SetAlpha(float _Alpha)
{
	if (GetOwner()->MeshRender() != nullptr
		&& GetOwner()->MeshRender()->GetMaterial() != nullptr)
	{
		GetOwner()->MeshRender()->GetMaterial()->SetScalar(SCALAR_PARAM::FLOAT_0, _Alpha);
	}
}
