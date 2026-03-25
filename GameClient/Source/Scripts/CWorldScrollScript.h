#pragma once
#include "CScript.h"
#include "AStageData.h"

struct FActiveSpawnInfo
{
	int				SpawnIndex;		// StageData 내 인덱스
	EObjectID		ObjectID;
	Ptr<GameObject>	Object;
};

class CWorldScrollScript : public CScript
{
private:
	// 스크롤 설정
	float		m_ScrollSpeed;			// 월드 스크롤 속도
	float		m_WorldOffset;			// 현재 월드 오프셋 (스크롤된 거리)

	// 스폰 영역 설정
	float		m_SpawnDistance;		// 플레이어 기준 오른쪽으로 얼마나 앞서서 스폰할지
	float		m_DespawnDistance;		// 플레이어 기준 왼쪽으로 얼마나 지나면 회수할지
	float		m_PlayerPosX;			// 플레이어 X 위치 (고정)

	// StageData
	Ptr<AStageData>		m_StageData;
	int					m_NextSpawnIndex;	// 다음 스폰할 SpawnInfo 인덱스

	// 활성화된 오브젝트 추적
	list<FActiveSpawnInfo>	m_listActiveObjects;

public:
	void SetScrollSpeed(float _Speed) { m_ScrollSpeed = _Speed; }
	void SetStageData(Ptr<AStageData> _StageData);
	void Reset();	// 스테이지 재시작 시 호출

	float GetWorldOffset() const { return m_WorldOffset; }
	float GetScrollSpeed() const { return m_ScrollSpeed; }

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

private:
	void UpdateScroll();
	void SpawnObjects();
	void DespawnObjects();
	void MoveActiveObjects();

	float WorldToScreenX(float _WorldX) const;

	CLONE(CWorldScrollScript);

public:
	CWorldScrollScript();
	virtual ~CWorldScrollScript();
};
