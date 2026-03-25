#include "pch.h"
#include "CEffectScript.h"

#include "TimeMgr.h"
#include "ObjectPoolMgr.h"
#include "GameObject.h"
#include "CMeshRender.h"

CEffectScript::CEffectScript()
	: CScript(SCRIPT_TYPE::EFFECTSCRIPT)
	, m_Duration(0.5f)
	, m_AccTime(0.f)
	, m_IsAlphaEffect(true)
	, m_InitialAlpha(1.f)
{
}

CEffectScript::~CEffectScript()
{
}

void CEffectScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsAlphaEffect, L"IsAlphaEffect", true);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitialAlpha, L"InitialAlpha", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, L"Duration", true, 0.f);
}

void CEffectScript::Begin()
{
	// 시작 시 초기화
	m_AccTime = 0.f;

	// 시작 시 알파값 1.0 (완전 불투명)으로 설정
	if (GetOwner()->MeshRender() != nullptr && GetOwner()->MeshRender()->GetMaterial() != nullptr)
	{
		GetOwner()->MeshRender()->GetMaterial()->SetScalar(SCALAR_PARAM::FLOAT_0, m_InitialAlpha);
	}
}

void CEffectScript::OnSpawn()
{
	m_AccTime = 0.f;

	// 플립북이 있으면 재생
	if (GetOwner()->FlipbookRender().Get() != nullptr)
	{
		GetOwner()->FlipbookRender()->Play(0, 12.f, false); // 1회 재생
	}
}

void CEffectScript::Tick()
{
	m_AccTime += DT;

	if (m_IsAlphaEffect)
	{
		// 페이드 아웃: 1.0 -> 0.0 으로 점점 투명해짐
		float alpha = m_InitialAlpha - (m_AccTime / m_Duration);
		if (alpha < 0.f) alpha = 0.f;

		GetOwner()->MeshRender()->GetMaterial()->SetScalar(SCALAR_PARAM::FLOAT_0, alpha);
	}


	// 시간이 지나면 풀에 반환
	if (m_AccTime >= m_Duration)
	{
		if (!m_PoolKey.empty())
		{
			ObjectPoolMgr::GetInst()->Return(m_PoolKey, GetOwner());
		}
		else
		{
			GetOwner()->SetActive(false);
		}
	}
}

void CEffectScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_IsAlphaEffect, sizeof(bool), 1, _File);
	fwrite(&m_InitialAlpha, sizeof(float), 1, _File);
	fwrite(&m_Duration, sizeof(float), 1, _File);
}

void CEffectScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_IsAlphaEffect, sizeof(bool), 1, _File);
	fread(&m_InitialAlpha, sizeof(float), 1, _File);
	fread(&m_Duration, sizeof(float), 1, _File);
}
