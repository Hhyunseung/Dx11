#include "pch.h"
#include "CEffectScript.h"

#include "TimeMgr.h"
#include "ObjectPoolMgr.h"
#include "GamePlayMgr.h"
#include "GameObject.h"
#include "CMeshRender.h"

CEffectScript::CEffectScript()
	: CScript(SCRIPT_TYPE::EFFECTSCRIPT)
	, m_Duration(0.5f)
	, m_AccTime(0.f)
	, m_IsAlphaEffect(true)
	, m_InitialAlpha(1.f)
	, m_IsScrolling(false)
{
}

CEffectScript::~CEffectScript()
{
}

void CEffectScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsAlphaEffect, L"IsAlphaEffect");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_InitialAlpha, L"InitialAlpha", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Duration, L"Duration", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::BOOL, &m_IsScrolling, L"IsScrolling");
}

void CEffectScript::Begin()
{
	// 시작 시 초기화
	m_AccTime = 0.f;

	// 이펙트 셰이더: 깊이 테스트 유지(LESS) + 깊이 쓰기 비활성화
	// → 캐릭터/배경 앞뒤 관계는 정상 유지, 이펙트끼리 겹쳐도 서로 차단하지 않음
	//auto applyLessNoWrite = [](CRenderComponent* pRender) {
	//	if (pRender == nullptr) return;
	//	auto pMtrl = pRender->GetMaterial();
	//	if (pMtrl == nullptr) return;
	//	auto pShader = pMtrl->GetShader();
	//	if (pShader == nullptr) return;
	//	pShader->SetDSType(DS_TYPE::LESS_NO_WRITE);
	//};
	//applyLessNoWrite(GetOwner()->FlipbookRender().Get());
	//applyLessNoWrite(GetOwner()->MeshRender().Get());

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
	if (GamePlayMgr::GetInst()->IsPaused())
		return;

	m_AccTime += DT;

	// 월드 스크롤과 함께 이동 (GamePlayMgr에서 속도 읽기)
	if (m_IsScrolling)
	{
		float scrollSpeed = GamePlayMgr::GetInst()->GetScrollSpeed();
		Vec3 pos = Transform()->GetRelativePos();
		pos.x -= scrollSpeed * DT;
		Transform()->SetRelativePos(pos);
	}

	// 플립북이 아닐 경우, 알파값을 점점 줄여서 페이드 아웃 효과
	if (m_IsAlphaEffect && (GetOwner()->FlipbookRender().Get() == nullptr))
	{
		if (GetOwner()->MeshRender() == nullptr || GetOwner()->MeshRender()->GetMaterial() == nullptr)
			return;

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
