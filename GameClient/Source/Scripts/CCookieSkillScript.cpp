#include "pch.h"
#include "CCookieSkillScript.h"

#include "TimeMgr.h"

CCookieSkillScript::CCookieSkillScript(SCRIPT_TYPE _Type)
	: CScript(_Type)
	, m_Player(nullptr)
	, m_WaitTime(12.f)   // 기본 자동 발동 대기시간
	, m_WaitAcc(0.f)
	, m_IsUsingSkill(false)
{
}

CCookieSkillScript::CCookieSkillScript()
	: CScript(SCRIPT_TYPE::COOKIESKILLSCRIPT)
{
}

CCookieSkillScript::~CCookieSkillScript()
{
}

void CCookieSkillScript::Begin()
{
}

void CCookieSkillScript::Tick()
{
	TickSkill();
}

bool CCookieSkillScript::CanAutoUseSkill() const
{
	if (m_Player == nullptr)
		return false;

	if (m_IsUsingSkill)
		return false;

	return true;
}

void CCookieSkillScript::TickSkill()
{
	if (m_Player == nullptr)
		return;

	if (m_IsUsingSkill)
		return;

	m_WaitAcc += DT;

	if (m_WaitAcc >= m_WaitTime)
	{
		if (CanAutoUseSkill())
		{
			m_WaitAcc = 0.f;
			UseSkill();
		}
	}
}
