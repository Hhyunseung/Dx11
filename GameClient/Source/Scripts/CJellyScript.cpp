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
	if (GamePlayMgr::GetInst()->IsPaused())
		return;

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

	// 월드 좌표로 방향/거리 계산 (젤리가 부모 그룹의 자식일 수 있으므로)
	Vec3 playerPos = pPlayer->Transform()->GetWorldPos();
	Vec3 myWorldPos = GetOwner()->Transform()->GetWorldPos();

	// 플레이어 오른쪽(진행 방향)으로 오프셋 → 캐릭터 앞에서 바로 먹을 수 있도록
	const Vec3 magnetTargetOffset(20.f, -50.f, 0.f);
	Vec3 targetPos = playerPos + magnetTargetOffset;

	Vec3 dir = targetPos - myWorldPos;
	float dist = dir.Length();

	if (dist <= pPlayer->GetMagnetRange() && dist > 1.f)
	{
		dir.Normalize();
		// 이동 델타는 로컬 좌표에 그대로 적용 (부모가 회전 없이 이동만 하므로 동일)
		Vec3 relPos = GetOwner()->Transform()->GetRelativePos();
		relPos += dir * m_MagnetSpeed * DT;
		GetOwner()->Transform()->SetRelativePos(relPos);
	}

}

void CJellyScript::BeginOverlap(CCollider2D* _This, CCollider2D* _Other)
{
	// 플레이어와 충돌했을 때의 처리
	// 점수 추가
	GamePlayMgr::GetInst()->AddScore(m_Score);

	// 코인 타입이면 RunCoin 카운트도 추가
	if (m_ObjectID == EObjectID::Jelly_SilverCoin)
	{
		GamePlayMgr::GetInst()->AddRunCoin(10);
	}

	if (m_ObjectID == EObjectID::Jelly_GoldCoin)
	{
		GamePlayMgr::GetInst()->AddRunCoin(50);
	}

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
	Vec3 pos = Transform()->GetWorldPos();
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
