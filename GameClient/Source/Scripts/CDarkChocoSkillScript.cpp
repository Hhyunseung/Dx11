#include "pch.h"
#include "CDarkChocoSkillScript.h"

#include "CFlipbookRender.h"
#include "AssetMgr.h"

CDarkChocoSkillScript::CDarkChocoSkillScript()
	: CCookieSkillScript(SCRIPT_TYPE::DARKCHOCOSKILLSCRIPT)
	, m_Duration(10.f)
	, m_RemainDuration(0.f)
	, m_AttackCooldown(1.0f)
	, m_AttackCOoldownTimer(0.f)
	, m_IsAttacking(false)
	, m_SkillState(ESkillState::None)
	, m_ScoreTickAcc(0.f)
	, m_ScoreTickCount(0)
	, m_MaxScoreTickCount(10)
	, m_ScorePerTick(1000)
{
}

CDarkChocoSkillScript::~CDarkChocoSkillScript()
{
}

void CDarkChocoSkillScript::Init()
{
	//m_SkillRun			= FIND(AFlipbook, L"Flipbook\\Pet_Default.flip");
	//m_SkillJump			= FIND(AFlipbook, L"Flipbook\\Pet_Default.flip");
	//m_SkillDoubleJump	= FIND(AFlipbook, L"Flipbook\\Pet_Default.flip");
	//m_SkillSlide		= FIND(AFlipbook, L"Flipbook\\Pet_Default.flip");
	//m_SkillAttack		= FIND(AFlipbook, L"Flipbook\\Pet_Default.flip");

}

void CDarkChocoSkillScript::Begin()
{
}

void CDarkChocoSkillScript::Tick()
{
}


void CDarkChocoSkillScript::OnEquip()
{
}

void CDarkChocoSkillScript::OnUnequip()
{
}

bool CDarkChocoSkillScript::CanAutoUseSkill() const
{
	return false;
}

void CDarkChocoSkillScript::UseSkill()
{
}

void CDarkChocoSkillScript::EndSkill()
{
}

void CDarkChocoSkillScript::TickSkill()
{
}

void CDarkChocoSkillScript::SaveToLevelFile(FILE* _File)
{
}

void CDarkChocoSkillScript::LoadFromLevelFile(FILE* _File)
{
}

