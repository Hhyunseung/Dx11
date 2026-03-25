#include "pch.h"
#include "CEffectScript.h"

#include "TimeMgr.h"
#include "ObjectPoolMgr.h"
#include "GameObject.h"

CEffectScript::CEffectScript()
	: CScript(SCRIPT_TYPE::EFFECTSCRIPT)
	, m_Duration(0.5f)
	, m_AccTime(0.f)
{
}

CEffectScript::~CEffectScript()
{
}

void CEffectScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, L"Duration", true, 0.f);
}

void CEffectScript::Begin()
{
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
	fwrite(&m_Duration, sizeof(float), 1, _File);
}

void CEffectScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Duration, sizeof(float), 1, _File);
}
