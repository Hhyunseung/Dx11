#include "pch.h"
#include "CObstructScript.h"

#include "TimeMgr.h"
#include "GamePlayMgr.h"
#include "GameObject.h"
#include "ObjectPoolMgr.h"

#include "CPlayerScript.h"

CObstructScript::CObstructScript(SCRIPT_TYPE _Type)
	: CScript(_Type)
	, m_Damage(30)
	, m_ObjectID(EObjectID::Obstacle_bl1)
	, m_IsBroken(false)
{
}

void CObstructScript::ApplySpawnInfo(const FSpawnInfo& info)
{
	// 기본적으로 CObstructScript는 Damage 및 ObjectID를 적용할 수 있음
	auto itInt = info.IntParams.find("Damage");
	if (itInt != info.IntParams.end())
		m_Damage = itInt->second;

	// ObjectID는 int로 저장됨
	if (info.FloatParams.find("ObjectID") != info.FloatParams.end())
	{
		m_ObjectID = (EObjectID)(int)info.FloatParams.at("ObjectID");
	}
}

CObstructScript::CObstructScript()
	: CScript(SCRIPT_TYPE::OBSTRUCTSCRIPT)
	, m_Damage(30)
	, m_ObjectID(EObjectID::Obstacle_bl1)
	, m_IsBroken(false)
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
	m_IsBroken = false;

	// MeshRender와 FlipbookRender가 있는지 먼저 확인
	if (GetOwner()->FlipbookRender() != nullptr)
	{
		GetOwner()->FlipbookRender()->Play(0, 12.f, -1);
	}

	// 스폰될 때마다 재설정해야하는 부분들
	// 충돌 콜백 등록 (풀에서 재사용될 때도 호출해야 함)
	// 콜라이더가 없는 오브젝트(예: CPendulumObstacle 피벗)는 등록 생략
	// 자식 콜라이더가 있는 경우 자식 클래스의 OnSpawn()에서 직접 등록
	if (GetOwner()->Collider2D() != nullptr)
	{
		ADD_DYNAMIC_BEGIN_OVERLAP(CObstructScript::BeginOverlap);
	}
}

void CObstructScript::Move()
{
	// 기본 구현: 월드 스크롤만 적용
	// ApplyWorldScroll();
}


void CObstructScript::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	CPlayerScript* pPlayer = GamePlayMgr::GetInst()->GetPlayerScript();
	if (pPlayer == nullptr || m_IsBroken)
		return;

	if (pPlayer->GetIsGiant() || pPlayer->GetIsBoost())
	{
		BreakObstacle();
		return;
	}

	pPlayer->TakeDamage(m_Damage);
}

void CObstructScript::BreakObstacle()
{
	if (m_IsBroken)
		return;

	m_IsBroken = true;

	Vec3 pos = Transform()->GetRelativePos();
	ObjectPoolMgr::GetInst()->SpawnEffect(L"Prefab\\Effect_Obstrcut_Break.pref", pos, true);

	ObjectPoolMgr::GetInst()->Return(m_ObjectID, GetOwner());
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


