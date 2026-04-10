#pragma once
#include "CScript.h"

class CPlayerScript;

class CCookieSkillScript :
    public CScript
{
protected:
	CPlayerScript* m_Player;

	Ptr<APrefab> m_SkillEffectPrefab;   // 스킬 발사체 프리팹 
	GameObject*  m_SkillEffectObj;      // 스킬 발사체 오브젝트

	Ptr<APrefab> m_SkillBGPrefab;       // 스킬 배경 프리팹
	GameObject*  m_SkillBGObject;       // 현재 씬에 생성된 스킬 배경 오브젝트 (없으면 nullptr)

	Ptr<APrefab> m_SkillBGEffectPrefab; // 스킬 배경 이펙트 프리팹 (1회 재생 후 소멸)
	GameObject*  m_SkillBGEffectObject; // 현재 씬에 생성된 스킬 배경 이펙트 오브젝트

	bool m_IsUsingSkill;

	float m_WaitTime;
	float m_WaitAcc;

public:
	void SetPlayer(CPlayerScript* _Player) { m_Player = _Player; }
	CPlayerScript* GetPlayer() const { return m_Player; }

	void SetSKillEffectPrefab(Ptr<APrefab> _Prefab) { m_SkillEffectPrefab = _Prefab; }
	Ptr<APrefab> GetSKillEffectPrefab() { return m_SkillEffectPrefab; }

	void SetSkillBGPrefab(Ptr<APrefab> _Prefab) { m_SkillBGPrefab = _Prefab; }
	Ptr<APrefab> GetSkillBGPrefab() { return m_SkillBGPrefab; }

	void SetSkillBGEffectPrefab(Ptr<APrefab> _Prefab) { m_SkillBGEffectPrefab = _Prefab; }
	Ptr<APrefab> GetSkillBGEffectPrefab() { return m_SkillBGEffectPrefab; }

	bool IsUsingSkill() const { return m_IsUsingSkill; }

public:
	virtual void OnEquip() {}
	virtual void OnUnequip() {}
	
	virtual bool CanAutoUseSkill() const;
	virtual void UseSkill() {};
	virtual void EndSkill() {};
	virtual void TickSkill();

	virtual bool IsSkillMovementMode() const { return false; }

protected:
	void SpawnSkillBG(int _LayerIdx);  // UseSkill() 시 호출
	void DestroySkillBG();             // EndSkill() 시 호출

	void SpawnSkillBGEffect(int _LayerIdx);  // UseSkill() 시 호출
	void DestroySkillBGEffect();             // EndSkill() 시 호출

	void SpawnSkillEffect(int _LayerIdx);  // UseSkill() 시 호출
	void DestroySkillEffect();             // EndSkill() 시 호출

public:
	virtual void Begin() override;
	virtual void Tick() override;

	CLONE(CCookieSkillScript);

public:
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

public:
	CCookieSkillScript(SCRIPT_TYPE _Type); // 자식 클래스용 생성자
	CCookieSkillScript();
	virtual ~CCookieSkillScript();
};

