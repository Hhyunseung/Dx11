#pragma once
#include "CCookieSkillScript.h"
class CDarkChocoSkillScript :
    public CCookieSkillScript
{
private:
	// 스킬 지속 시간
	float m_Duration;
	float m_RemainDuration;

	// 공격
	float m_AttackCooldown;
	float m_AttackCOoldownTimer;
	bool  m_IsAttacking;

	ESkillState m_SkillState;

	// 스킬 전용 플립북
	Ptr<AFlipbook> m_SkillRun;
	Ptr<AFlipbook> m_SkillJump;
	Ptr<AFlipbook> m_SkillDoubleJump;
	Ptr<AFlipbook> m_SkillSlide;
	Ptr<AFlipbook> m_SkillAttack;

	// 점수
	float m_ScoreTickAcc;
	int   m_ScoreTickCount;
	int   m_MaxScoreTickCount;
	int   m_ScorePerTick;

public:
	virtual void OnEquip() override;
	virtual void OnUnequip() override;

	virtual bool CanAutoUseSkill() const;
	virtual void UseSkill() override;
	virtual void EndSkill() override;
	virtual void TickSkill() override;

	virtual bool IsSkillMovementMode() const override { return false; }

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

private:


public:
	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _File) override;

	CLONE(CDarkChocoSkillScript);

public:
	CDarkChocoSkillScript();
	virtual ~CDarkChocoSkillScript();
};

