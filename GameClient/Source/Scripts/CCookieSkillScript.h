#pragma once
#include "CScript.h"

class CPlayerScript;

class CCookieSkillScript :
    public CScript
{
protected:
	CPlayerScript* m_Player;

	bool m_IsUsingSkill;

	float m_WaitTime;
	float m_WaitAcc;

public:
	void SetPlayer(CPlayerScript* _Player) { m_Player = _Player; }
	CPlayerScript* GetPlayer() const { return m_Player; }

	bool IsUsingSkill() const { return m_IsUsingSkill; }

public:
	virtual void OnEquip() {}
	virtual void OnUnequip() {}

	virtual bool CanAutoUseSkill() const;
	virtual void UseSkill() {};
	virtual void EndSkill() {};
	virtual void TickSkill();

	virtual bool IsSkillMovementMode() const { return false; }

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

