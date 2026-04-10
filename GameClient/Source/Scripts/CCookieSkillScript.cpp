#include "pch.h"
#include "CCookieSkillScript.h"

#include "GamePlayMgr.h"
#include "TimeMgr.h"
#include "TaskMgr.h"
#include "APrefab.h"
#include "CSkillBGScript.h"

CCookieSkillScript::CCookieSkillScript(SCRIPT_TYPE _Type)
	: CScript(_Type)
	, m_Player(nullptr)
	, m_SkillBGPrefab(nullptr)
	, m_SkillBGObject(nullptr)
	, m_SkillBGEffectPrefab(nullptr)
	, m_SkillBGEffectObject(nullptr)
	, m_SkillEffectPrefab(nullptr)
	, m_SkillEffectObj(nullptr)
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
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_SkillBGPrefab, L"SkillBGPrefab");
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_SkillBGEffectPrefab, L"SkillBGEffectPrefab");
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_SkillEffectPrefab, L"SkillEffectPrefab");
}

void CCookieSkillScript::Tick()
{
	if (GamePlayMgr::GetInst()->IsPaused())
		return;

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

void CCookieSkillScript::SpawnSkillBG(int _LayerIdx)
{
	if (m_SkillBGPrefab == nullptr)
		return;

	DestroySkillBG(); // 혹시 이전 것이 남아있으면 제거

	m_SkillBGObject = m_SkillBGPrefab->Instantiate();
	CreateObject(m_SkillBGObject, _LayerIdx);
}

void CCookieSkillScript::DestroySkillBG()
{
	if (m_SkillBGObject == nullptr || m_SkillBGObject->IsDead())
	{
		m_SkillBGObject = nullptr;
		return;
	}

	Ptr<CSkillBGScript> pBGScript = m_SkillBGObject->GetScript<CSkillBGScript>();
	if (pBGScript != nullptr)
	{
		pBGScript->BeginFadeOut();
	}
	else
	{
		TaskInfo info = {};
		info.Type    = TASK_TYPE::DESTROY_OBJECT;
		info.Param_0 = (DWORD_PTR)m_SkillBGObject;
		TaskMgr::GetInst()->AddTask(info);
	}
	m_SkillBGObject = nullptr;
}

void CCookieSkillScript::SpawnSkillBGEffect(int _LayerIdx)
{
	if (m_SkillBGEffectPrefab == nullptr)
		return;

	DestroySkillBGEffect();

	m_SkillBGEffectObject = m_SkillBGEffectPrefab->Instantiate();
	CreateObject(m_SkillBGEffectObject, _LayerIdx);
}

void CCookieSkillScript::DestroySkillBGEffect()
{
	if (m_SkillBGEffectObject == nullptr || m_SkillBGEffectObject->IsDead())
	{
		m_SkillBGEffectObject = nullptr;
		return;
	}

	TaskInfo info = {};
	info.Type    = TASK_TYPE::DESTROY_OBJECT;
	info.Param_0 = (DWORD_PTR)m_SkillBGEffectObject;
	TaskMgr::GetInst()->AddTask(info);
	m_SkillBGEffectObject = nullptr;
}

void CCookieSkillScript::SpawnSkillEffect(int _LayerIdx)
{
	if (m_SkillEffectPrefab == nullptr)
		return;

	DestroySkillEffect();

	m_SkillEffectObj = m_SkillEffectPrefab->Instantiate();
	m_SkillEffectObj->Transform()->SetRelativePos(m_Player->GetOwner()->Transform()->GetRelativePos());
	CreateObject(m_SkillEffectObj, _LayerIdx);
}

void CCookieSkillScript::DestroySkillEffect()
{
	if (m_SkillEffectObj == nullptr || m_SkillEffectObj->IsDead())
	{
		m_SkillEffectObj = nullptr;
		return;
	}

	TaskInfo info = {};
	info.Type = TASK_TYPE::DESTROY_OBJECT;
	info.Param_0 = (DWORD_PTR)m_SkillEffectObj;
	TaskMgr::GetInst()->AddTask(info);
	m_SkillEffectObj = nullptr;
}
