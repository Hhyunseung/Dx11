#pragma once
#include "CScript.h"

class CEffectScript : public CScript
{
private:
	float		m_Duration;			// 이펙트 지속 시간
	float		m_AccTime;			// 경과 시간
	bool		m_IsAlphaEffect;    // 점점 투명해지는 효과 여부
	float		m_InitialAlpha;		// 시작 시 알파값 (예: 1.0f) /// 1.0 -> 0.0

	bool		m_IsScrolling;		// 월드 스크롤과 함께 이동 여부

	wstring		m_PoolKey;			// 풀 반환용 키

public:
	void SetDuration(float _Duration) { m_Duration = _Duration; }
	void SetPoolKey(const wstring& _Key) { m_PoolKey = _Key; }
	void SetScrolling(bool _Scrolling) { m_IsScrolling = _Scrolling; }

	// 이펙트 활성화 시 호출 (풀에서 가져올 때)
	void OnSpawn();

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CEffectScript);

public:
	CEffectScript();
	virtual ~CEffectScript();
};
