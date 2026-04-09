#pragma once
#include "CScript.h"

enum class EItemType
{
	Giant,
	Boost,
	Magnet,
	HealHP,
};


class CItemScript :
    public CScript
{
private:
	EItemType		m_ItemType;			// 아이템 종류
	float			m_Duration;
	int				m_HealAmount;

	EObjectID		m_ObjectID;			// 풀링용 ID
	Ptr<APrefab>	m_CollectEffect;	// 수집 효과 프리팹
	bool 			m_bSpawned;			// 스폰 초기화 여부 체크

public:
	void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);
	virtual void OnSpawn() override;

	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

private:
	void SpawnCollectEffect();
	void ReturnToPool();

public:
	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CItemScript);

public:
	CItemScript();
	virtual ~CItemScript();
};

