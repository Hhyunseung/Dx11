#pragma once
#include "CCookieSkillScript.h"

enum class ETimeKeeperSkillState
{
	None,
	Start,
	Loop,
	Slide,
	End,
};


class CTimeKeeperScript :
    public CCookieSkillScript
{
private:
	// 스킬 시간
	float m_Duration;
	float m_RemainDuration;

	// 이동
	float m_UpSpeed;
	float m_DownSpeed;

	float m_MaxUpOffset;
	float m_MaxDownOffset;
	bool  m_InitSkillPos;
	float m_StartY;

	// 애니메이션 상태
	ETimeKeeperSkillState m_SkillState;
	bool  m_IsChargeMotion;
	
	// 점수
	float m_ScoreTickAcc;
	int   m_ScoreTickCount;
	int   m_MaxScoreTickCount;
	int   m_ScorePerTick;

	// 스킬 종료 예약 여부
	bool m_bEndReserved; 

public:
	virtual void OnEquip() override;
	virtual void OnUnequip() override;

	virtual bool CanAutoUseSkill() const override;
	virtual void UseSkill() override;
	virtual void EndSkill() override;
	virtual void TickSkill() override;

	// 스킬 모드
	virtual bool IsSkillMovementMode() const override { return m_IsUsingSkill; }

private:
	void EnterSkillMode();
	void ExitSkillMode();


	void UpdateSkillMove();			// 상하 이동
	void UpdateSkillScore();		// 점수 처리
	void UpdateSkillAnimState();	// 상태 전환

	// 상태 전환
	void ChangeSkillState(ETimeKeeperSkillState _NextState);
	void PlaySkillAnim(ETimeKeeperSkillState _AnimState);

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

