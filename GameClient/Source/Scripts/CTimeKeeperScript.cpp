#include "pch.h"
#include "CTimeKeeperScript.h"

#include "CPlayerScript.h"
#include "GameObject.h"

#include "GamePlayMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

CTimeKeeperScript::CTimeKeeperScript()
	: CCookieSkillScript(SCRIPT_TYPE::TIMEKEEPERSCRIPT)
	, m_Duration(10.f)           // 스킬 활성 시간
	, m_RemainDuration(0.f)
	, m_UpSpeed(500.f)
	, m_DownSpeed(500.f)
	, m_InitSkillPos(false)
	, m_StartY(0.f)
	, m_MaxUpOffset(250.f)
	, m_MaxDownOffset(250.f)
	, m_IsChargeMotion(false)
	, m_ScoreTickAcc(0.f)
	, m_ScoreTickCount(0)
	, m_MaxScoreTickCount(10)
	, m_ScorePerTick(30000)
{
	m_WaitTime = 3.f; // 자동 발동 대기시간 15초
}


CTimeKeeperScript::~CTimeKeeperScript()
{
}

// 스킬 장착 시 호출
void CTimeKeeperScript::OnEquip()
{
}

// 스킬 해제 시 호출
void CTimeKeeperScript::OnUnequip()
{
	EndSkill();
}


bool CTimeKeeperScript::CanAutoUseSkill() const
{
	if (!CCookieSkillScript::CanAutoUseSkill())
		return false;

	return true;
}

// 스킬 사용 시 호출
void CTimeKeeperScript::UseSkill()
{
	if (!CanAutoUseSkill())
		return;

	m_IsUsingSkill = true;
	m_RemainDuration = m_Duration;

	m_InitSkillPos = false;
	m_IsChargeMotion = false;

	m_ScoreTickAcc = 0.f;
	m_ScoreTickCount = 0;

	EnterSkillMode();
}

void CTimeKeeperScript::EndSkill()
{
	if (!m_IsUsingSkill)
		return;

	m_IsUsingSkill = false;
	m_RemainDuration = 0.f;

	m_IsChargeMotion = false;
	m_ScoreTickAcc = 0.f;

	ExitSkillMode();
}

void CTimeKeeperScript::TickSkill()
{
	// 비활성화 상태에서 자동 발동 대기
	if (!m_IsUsingSkill)
	{
		CCookieSkillScript::TickSkill();
		return;
	}

	m_RemainDuration -= DT;

	UpdateSkillMove();
	UpdateSkillAnimState();
	UpdateSkillScore();
}


void CTimeKeeperScript::EnterSkillMode()
{
	if (m_Player == nullptr)
		return;

	m_Player->SetIsLand(false);
	m_Player->SetIsSkillMoveMode(true);

	m_Player->SetIsTimeKeeperSkillAnim2(false);
}

void CTimeKeeperScript::ExitSkillMode()
{
	if (m_Player == nullptr)
		return;

	m_Player->SetIsSkillMoveMode(false);
	m_Player->SetIsTimeKeeperSkillAnim2(false);
}


void CTimeKeeperScript::UpdateSkillMove()
{
	if (m_Player == nullptr)
		return;

	GameObject* pObj = m_Player->GetOwner();
	if (pObj == nullptr)
		return;

	Vec3 vPos = pObj->Transform()->GetRelativePos();

	if (!m_InitSkillPos)
	{
		m_StartY = vPos.y;
		m_InitSkillPos = true;
	}

	bool bJumpPressed = KEY_PRESSED(KEY::SPACE);

	// 점프키 누르면 위로, 아니면 아래로
	if (bJumpPressed)
	{
		vPos.y += m_UpSpeed * DT;
	}
	else
	{
		vPos.y -= m_DownSpeed * DT;
	}

	float maxY = m_StartY + m_MaxUpOffset;
	float minY = m_StartY - m_MaxDownOffset;

	if (vPos.y > maxY)
		vPos.y = maxY;

	if (vPos.y < minY)
		vPos.y = minY;

	pObj->Transform()->SetRelativePos(vPos);
}

void CTimeKeeperScript::UpdateSkillAnimState()
{
	if (m_Player == nullptr)
		return;

	bool bSlidePressed = KEY_PRESSED(KEY::DOWN);

	m_IsChargeMotion = bSlidePressed;
	m_Player->SetIsTimeKeeperSkillAnim2(bSlidePressed);
}

void CTimeKeeperScript::UpdateSkillScore()
{
	if (!m_IsChargeMotion)
	{
		m_ScoreTickAcc = 0.f;
		return;
	}

	if (m_ScoreTickCount >= m_MaxScoreTickCount)
		return;

	m_ScoreTickAcc += DT;

	while (m_ScoreTickAcc >= 1.f && m_ScoreTickCount < m_MaxScoreTickCount)
	{
		m_ScoreTickAcc -= 1.f;
		m_ScoreTickCount++;
		// 점수 획득 처리 (예: 플레이어 점수 증가)

		GamePlayMgr::GetInst()->AddScore(m_ScorePerTick);
	}
}
