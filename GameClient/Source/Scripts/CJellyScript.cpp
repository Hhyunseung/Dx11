#include "pch.h"
#include "CJellyScript.h"

#include "GamePlayMgr.h"
#include "TimeMgr.h"
#include "ObjectPoolMgr.h"
#include "GameObject.h"


CJellyScript::CJellyScript()
	: CScript(SCRIPT_TYPE::JELLYSCRIPT)
	, m_Score(1000)
	, m_Magnet(true)
	, m_MagnetRange(300.f)
	, m_MagnetSpeed(500.f)
	, m_ObjectID(EObjectID::DefaultJelly)
{

}

CJellyScript::~CJellyScript()
{
}


void CJellyScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_Score, L"Score", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::EObjectID, &m_ObjectID, L"ObjectID", true, 0.f);
	//AddScriptParam(SCRIPT_PARAM::TEXTURE, &m_Tex, L"ChangeTex");
}

void CJellyScript::Begin()
{
	// MeshRender와 FlipbookRender가 있는지 먼저 확인
	if (GetOwner()->FlipbookRender() != nullptr)
	{
		GetOwner()->FlipbookRender()->Play(0, 12.f, -1);
	}


	ADD_DYNAMIC_BEGIN_OVERLAP(CJellyScript::BeginOverlap);
}

void CJellyScript::Tick()
{
	// 첫 Tick에서 스폰 초기화 (Begin이 호출 안 됐을 수 있음)
	if (!m_bSpawned)
	{
		OnSpawn();
		m_bSpawned = true;
	}

	// 플레이어가 자석 상태일 때, 플레이어가 범위 안에 있으면 젤리가 플레이어를 향해서 움직이는 코드
	CPlayerScript* pPlayer = GamePlayMgr::GetInst()->GetPlayerScript();
	if (pPlayer == nullptr)
		return;

	if (!pPlayer->GetIsMagnet())
		return;

	Vec3 playerPos = pPlayer->Transform()->GetRelativePos();
	Vec3 myPos = GetOwner()->Transform()->GetRelativePos();

	Vec3 dir = playerPos - myPos;
	float dist = dir.Length();

	if (dist <= pPlayer->GetMagnetRange() && dist > 1.f)
	{
		dir.Normalize();
		myPos += dir * m_MagnetSpeed * DT;
		GetOwner()->Transform()->SetRelativePos(myPos);
	}

}

void CJellyScript::BeginOverlap(CCollider2D* _This, CCollider2D* _Other)
{
	// 플레이어와 충돌했을 때의 처리
	// 점수 추가
	GamePlayMgr::GetInst()->AddScore(m_Score);

	// 이펙트 생성
	SpawnCollectEffect();

	// 오브젝트 풀에 반환 (비활성화)
	ReturnToPool();
}

void CJellyScript::OnSpawn()
{
}

void CJellyScript::SpawnCollectEffect()
{
	Vec3 pos = Transform()->GetRelativePos();
	ObjectPoolMgr::GetInst()->SpawnEffect(L"Prefab\\Effect_JellyCollect.pref", pos, true);
}

void CJellyScript::ReturnToPool()
{
	// 풀에 반환
	ObjectPoolMgr::GetInst()->Return(m_ObjectID, GetOwner());
}


void CJellyScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Score, sizeof(int), 1, _File);
	fwrite(&m_ObjectID, sizeof(EObjectID), 1, _File);
}

void CJellyScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Score, sizeof(int), 1, _File);
	fread(&m_ObjectID, sizeof(EObjectID), 1, _File);
}
