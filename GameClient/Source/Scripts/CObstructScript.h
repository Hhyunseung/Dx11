#pragma once
#include "CScript.h"

class CObstructScript : public CScript
{
private:
	EObjectID m_ObjectID;
	int	m_Damage;

public:
	void SetObjectID(EObjectID& _ID) { m_ObjectID = _ID; }
	const EObjectID& GetPoolKey() const { return m_ObjectID; }

	void SetDamage(int _Damage) { m_Damage = _Damage; }
	int	 GetDamage() const { return m_Damage; }

	void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

    // 스폰 시 호출 (자식에서 오버라이드 가능)
	virtual void OnSpawn();

	// Apply spawn-time overrides
	virtual void ApplySpawnInfo(const FSpawnInfo& info) override;

	// 이동 처리 (자식에서 오버라이드 가능, 기본은 월드 스크롤만)
	virtual void Move();

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CObstructScript);

public:
	// 자식 클래스용 생성자
	CObstructScript(SCRIPT_TYPE _Type);
	// 기본 생성자
	CObstructScript();
	virtual ~CObstructScript();
};

