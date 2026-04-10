#include "pch.h"
#include "CGamePlaySpawnScript.h"

#include "AssetMgr.h"
#include "GamePlayMgr.h"
#include "AStageData.h"

CGamePlaySpawnScript::CGamePlaySpawnScript()
	: CScript(SCRIPT_TYPE::GAMEPLAYSPAWNSCRIPT)
	, m_vecPlayerSpawnPos(-650.f, -95.f, 100.f, 0.f)
{
}

CGamePlaySpawnScript::~CGamePlaySpawnScript()
{
}

void CGamePlaySpawnScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::VEC4, &m_vecPlayerSpawnPos, L"VecSpawnPos", true, 0.f);
}

void CGamePlaySpawnScript::Begin()
{
	GamePlayMgr::GetInst()->Init();

	// 플레이어 스폰
	SpawnPlayer();

	SpawnUI();
	// StageData 기반 오브젝트 스폰
	// SpawnStageObjects();
}

void CGamePlaySpawnScript::Tick()
{

}

void CGamePlaySpawnScript::SpawnUI()
{
	Ptr<APrefab> pPrefabUI = GamePlayMgr::GetInst()->GetGamePlayUIRootPrefab();
	if (pPrefabUI == nullptr)
		return;

	GameObject* pUI = pPrefabUI->Instantiate();
	pUI->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f)); // UI는 월드 좌표 (0,0,0)에 배치
	CreateObject(pUI, 31); // UI 레이어
}

void CGamePlaySpawnScript::SpawnPlayer()
{
	Ptr<APrefab> pPrefab = GamePlayMgr::GetInst()->GetSelectedCharacterPrefab();
	if (pPrefab == nullptr)
		return;

	GameObject* pPlayer = pPrefab->Instantiate();
	pPlayer->Transform()->SetRelativePos(Vec3(m_vecPlayerSpawnPos.x, m_vecPlayerSpawnPos.y, m_vecPlayerSpawnPos.z));

	CreateObject(pPlayer, 0);
}

void CGamePlaySpawnScript::SpawnStageObjects()
{
	Ptr<AStageData> pStageData = GamePlayMgr::GetInst()->GetStageData();
	if (pStageData == nullptr)
		return;

	const vector<FSpawnInfo>& vecSpawnInfo = pStageData->GetSpawnInfo();

	for (const auto& info : vecSpawnInfo)
	{
		// ObjectID로 프리팹 키 가져오기
		wstring PrefabKey = GamePlayMgr::GetPrefabKey(info.ObjectID);
		if (PrefabKey.empty())
			continue;

		// AssetMgr에서 프리팹 찾기
		Ptr<APrefab> pPrefab = FIND(APrefab, PrefabKey);
		if (pPrefab == nullptr)
			continue;

		// 프리팹 인스턴스화
		GameObject* pObject = pPrefab->Instantiate();

        // 위치와 스케일 설정
		// 이전에는 Z를 하드코딩(700.f)하여 생성되어 항상 700으로 배치되었음.
		// 대신 프리팹 인스턴스의 기본 Z 값을 유지하도록 변경.
		float prefabZ = pObject->Transform()->GetRelativePos().z;
		pObject->Transform()->SetRelativePos(Vec3(info.WorldPos.x, info.WorldPos.y, prefabZ));
		pObject->Transform()->SetRelativeScale(Vec3(info.Scale.x, info.Scale.y, 1.f));


		const_cast<FSpawnInfo&>(info); // keep const correctness
		vector<Ptr<CScript>> vecScripts = pObject->GetScripts();
		for (auto& pScript : vecScripts)
		{
			pScript->ApplySpawnInfo(info);
		}

		// 레벨에 추가 (StageData에 저장된 LayerIdx 사용)
		CreateObject(pObject, info.LayerIdx);
	}
}

void CGamePlaySpawnScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vecPlayerSpawnPos, sizeof(Vec3), 1, _File);
}

void CGamePlaySpawnScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_vecPlayerSpawnPos, sizeof(Vec3), 1, _File);
}
