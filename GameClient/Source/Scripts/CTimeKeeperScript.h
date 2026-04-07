#pragma once
#include "CCookieSkillScript.h"

class CTimeKeeperScript :
    public CCookieSkillScript
{
private:
	float m_Duration;
	float m_RemainDuration;

	float m_UpSpeed;
	float m_DownSpeed;
	float m_MaxUpOffset;
	float m_MaxDownOffset;

	bool  m_InitSkillPos;
	float m_StartY;

	// 애니메이션 상태
	bool  m_IsChargeMotion;
	
	// 점수
	float m_ScoreTickAcc;
	int   m_ScoreTickCount;
	int   m_MaxScoreTickCount;
	int   m_ScorePerTick;

public:
	virtual void OnEquip() override;
	virtual void OnUnequip() override;

	virtual bool CanAutoUseSkill() const override;
	virtual void UseSkill() override;
	virtual void EndSkill() override;
	virtual void TickSkill() override;
	virtual bool IsSkillMovementMode() const override { return m_IsUsingSkill; }

private:
	void EnterSkillMode();
	void ExitSkillMode();


	void UpdateSkillMove();
	void UpdateSkillAnimState();
	void UpdateSkillScore();

public:
	bool IsChargeMotion() const { return m_IsChargeMotion; }

public:
	// 저장 불러오기
	virtual void SaveToLevelFile(FILE* _File) override {}
	virtual void LoadFromLevelFile(FILE* _File) override {}

	CLONE(CTimeKeeperScript);

public:
	CTimeKeeperScript();
	virtual ~CTimeKeeperScript();
};

