#pragma once
#include "CScript.h"

class CObstructScript : public CScript
{
private:
	wstring m_PoolKey;
	int	m_Damage;

public:
	void SetPoolKey(const wstring& _Key) { m_PoolKey = _Key; }
	const wstring& GetPoolKey() const { return m_PoolKey; }

	void SetDamage(int _Damage) { m_Damage = _Damage; }
	int	 GetDamage() const { return m_Damage; }

	void BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider);

	// 스폰 시 호출 (자식에서 오버라이드 가능)
	virtual void OnSpawn();

	// 이동 처리 (자식에서 오버라이드 가능, 기본은 월드 스크롤만)
	virtual void Move();

protected:
	// 공통 유틸리티: 월드 스크롤 적용
	void ApplyWorldScroll();

public:
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

