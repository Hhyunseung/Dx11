#include "pch.h"
#include "CObstructScript.h"

#include "TimeMgr.h"
#include "GamePlayMgr.h"
#include "GameObject.h"

#include "CPlayerScript.h"

CObstructScript::CObstructScript(SCRIPT_TYPE _Type)
	: CScript(_Type)
	, m_Damage(30)
{
}

CObstructScript::CObstructScript()
	: CScript(SCRIPT_TYPE::OBSTRUCTSCRIPT)
{
}

CObstructScript::~CObstructScript()
{
}

void CObstructScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_Damage, L"Damage", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::EObjectID, &m_ObjectID, L"ObjectID", true, 0.f);

}

void CObstructScript::Begin()
{
	// Begin은 레벨에 추가 시 호출됨 (풀에서는 호출 안 될 수 있음)
	// 충돌 콜백은 OnSpawn()에서 등록

}

void CObstructScript::Tick()
{
	// 자식의 Move() 호출
	Move();
}

void CObstructScript::OnSpawn()
{
	// MeshRender와 FlipbookRender가 있는지 먼저 확인
	if (GetOwner()->FlipbookRender() != nullptr)
	{
		GetOwner()->FlipbookRender()->Play(0, 12.f, -1);
	}

	// 스폰될 때마다 재설정해야하는 부분들
	// 충돌 콜백 등록 (풀에서 재사용될 때도 호출해야 함)
	ADD_DYNAMIC_BEGIN_OVERLAP(CObstructScript::BeginOverlap);
}

void CObstructScript::Move()
{
	// 기본 구현: 월드 스크롤만 적용
	// ApplyWorldScroll();
}


void CObstructScript::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	GamePlayMgr::GetInst()->GetPlayerScript()->TakeDamage(m_Damage);
}


void CObstructScript::SaveToLevelFile(FILE* _File)
{
	// 기본 클래스는 저장할 데이터 없음
	// 자식에서 오버라이드
}

void CObstructScript::LoadFromLevelFile(FILE* _File)
{
	// 기본 클래스는 로드할 데이터 없음
	// 자식에서 오버라이드
}


