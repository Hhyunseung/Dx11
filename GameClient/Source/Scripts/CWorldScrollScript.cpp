#include "pch.h"
#include "CWorldScrollScript.h"

#include "TimeMgr.h"
#include "GamePlayMgr.h"
#include "ObjectPoolMgr.h"
#include "AStageData.h"

CWorldScrollScript::CWorldScrollScript()
	: CScript(SCRIPT_TYPE::WORLDSCROLLSCRIPT)
	, m_ScrollSpeed(300.f)
	, m_WorldOffset(0.f)
	, m_SpawnDistance(800.f)		// 화면 오른쪽 밖
	, m_DespawnDistance(-200.f)		// 화면 왼쪽 밖
	, m_PlayerPosX(-650.f)			// 플레이어 고정 X 위치
	, m_NextSpawnIndex(0)
{
}

CWorldScrollScript::~CWorldScrollScript()
{
}

void CWorldScrollScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_ScrollSpeed, L"ScrollSpeed", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_SpawnDistance, L"SpawnDistance", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_DespawnDistance, L"DespawnDistance", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_PlayerPosX, L"PlayerPosX", true, 0.f);
}

void CWorldScrollScript::Begin()
{
	// GamePlayMgr에서 StageData 가져오기
	m_StageData = GamePlayMgr::GetInst()->GetStageData();
	Reset();
}

void CWorldScrollScript::Tick()
{
	UpdateScroll();
	SpawnObjects();
	MoveActiveObjects();
	DespawnObjects();
}

void CWorldScrollScript::SetStageData(Ptr<AStageData> _StageData)
{
	m_StageData = _StageData;
	Reset();
}

void CWorldScrollScript::Reset()
{
	m_WorldOffset = 0.f;
	m_NextSpawnIndex = 0;

	// 활성 오브젝트들 전부 반환
	for (auto& info : m_listActiveObjects)
	{
		ObjectPoolMgr::GetInst()->Return(info.ObjectID, info.Object.Get());
	}
	m_listActiveObjects.clear();
}

void CWorldScrollScript::UpdateScroll()
{
	m_WorldOffset += m_ScrollSpeed * DT;

	// 전역 스크롤 속도 동기화 (이펙트 등에서 사용)
	GamePlayMgr::GetInst()->SetScrollSpeed(m_ScrollSpeed);
}

void CWorldScrollScript::SpawnObjects()
{
	if (m_StageData == nullptr)
		return;

	const vector<FSpawnInfo>& vecSpawnInfo = m_StageData->GetSpawnInfo();

	// 스폰 기준점: 플레이어 위치 + 스폰 거리 + 월드 오프셋
	float spawnThreshold = m_PlayerPosX + m_SpawnDistance + m_WorldOffset;

	// 스폰해야 할 오브젝트들 확인
	while (m_NextSpawnIndex < (int)vecSpawnInfo.size())
	{
		const FSpawnInfo& info = vecSpawnInfo[m_NextSpawnIndex];

		// 아직 스폰 영역에 도달하지 않음
		if (info.WorldPos.x > spawnThreshold)
			break;

		// 오브젝트 풀에서 가져오기
		GameObject* pObject = ObjectPoolMgr::GetInst()->Get((EObjectID)info.ObjectID);
		if (pObject != nullptr)
		{
			// 화면상 위치 계산
			float screenX = WorldToScreenX(info.WorldPos.x);
			pObject->Transform()->SetRelativePos(Vec3(screenX, info.WorldPos.y, 700.f));
			pObject->Transform()->SetRelativeScale(Vec3(info.Scale.x, info.Scale.y, 1.f));

            // 활성 목록에 추가
			FActiveSpawnInfo activeInfo;
			activeInfo.SpawnIndex = m_NextSpawnIndex;
			activeInfo.ObjectID = (EObjectID)info.ObjectID;
			activeInfo.Object = pObject;

			for (Ptr<CScript>& pScript : pObject->GetScripts())
			{
				if (pScript != nullptr)
					pScript->ApplySpawnInfo(info);
			}

      const vector<Ptr<GameObject>>& children = pObject->GetChild();
		for (const Ptr<GameObject>& child : children)
		{
			if (child == nullptr)
				continue;

			// Apply spawn info to each script attached to the child object
			const vector<Ptr<CScript>>& childScripts = child->GetScripts();
			for (const Ptr<CScript>& pChildScript : childScripts)
			{
				if (pChildScript != nullptr)
					pChildScript->ApplySpawnInfo(info);
			}
		}

			m_listActiveObjects.push_back(activeInfo);
		}

		++m_NextSpawnIndex;
	}
}

void CWorldScrollScript::MoveActiveObjects()
{
	// 모든 활성 오브젝트를 왼쪽으로 이동 (스크롤 효과)
	float moveAmount = m_ScrollSpeed * DT;

	for (auto& info : m_listActiveObjects)
	{
		if (info.Object == nullptr)
			continue;

		Vec3 pos = info.Object->Transform()->GetRelativePos();
		pos.x -= moveAmount;
		info.Object->Transform()->SetRelativePos(pos);
	}
}

void CWorldScrollScript::DespawnObjects()
{
	// 화면 왼쪽을 벗어난 오브젝트 회수
	float despawnX = m_PlayerPosX + m_DespawnDistance;

	auto iter = m_listActiveObjects.begin();
	while (iter != m_listActiveObjects.end())
	{
		Vec3 pos = iter->Object->Transform()->GetRelativePos();

		if (pos.x < despawnX)
		{
			// 풀에 반환
			ObjectPoolMgr::GetInst()->Return(iter->ObjectID, iter->Object.Get());
			iter = m_listActiveObjects.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

float CWorldScrollScript::WorldToScreenX(float _WorldX) const
{
	// 월드 좌표를 화면 좌표로 변환
	// WorldX - WorldOffset = ScreenX (플레이어 기준 상대 위치)
	return _WorldX - m_WorldOffset;
}

void CWorldScrollScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_ScrollSpeed, sizeof(float), 1, _File);
	fwrite(&m_SpawnDistance, sizeof(float), 1, _File);
	fwrite(&m_DespawnDistance, sizeof(float), 1, _File);
	fwrite(&m_PlayerPosX, sizeof(float), 1, _File);
}

void CWorldScrollScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_ScrollSpeed, sizeof(float), 1, _File);
	fread(&m_SpawnDistance, sizeof(float), 1, _File);
	fread(&m_DespawnDistance, sizeof(float), 1, _File);
	fread(&m_PlayerPosX, sizeof(float), 1, _File);
}
