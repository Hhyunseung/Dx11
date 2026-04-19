#include "pch.h"
#include "CTimeKeeperScript.h"

#include "CPlayerScript.h"
#include "CTimeKeeperBGEffectScript.h"
#include "GameObject.h"

#include "AssetMgr.h"
#include "GamePlayMgr.h"
#include "SceneFlowMgr.h"
#include "TaskMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

CTimeKeeperScript::CTimeKeeperScript()
	: CCookieSkillScript(SCRIPT_TYPE::TIMEKEEPERSCRIPT)
	, m_Duration(10.f)           // 스킬 활성 시간
	, m_RemainDuration(0.f)
	, m_UpSpeed(300.f)
	, m_DownSpeed(300.f)
	, m_InitSkillPos(false)
	, m_StartY(0.f)
	, m_MinSkillY(-150.f)
	, m_MaxSkillY(300.f)
	, m_IsChargeMotion(false)
	, m_ScoreTickAcc(0.f)
	, m_ScoreTickCount(0)
	, m_MaxScoreTickCount(10)
	, m_ScorePerTick(3000000)
	, m_TKBGEffectPrefab(nullptr)
	, m_TKBGEffectObject(nullptr)
{
	m_WaitTime = 15.f; // 자동 발동 대기시간 15초
}


CTimeKeeperScript::~CTimeKeeperScript()
{
}

// 스킬 장착 시 호출
void CTimeKeeperScript::OnEquip()
{
	//m_Player->GetStateMachine()->AddState(new CRunState(m_Player));

	Ptr<APrefab> pPrefab = FIND(APrefab, L"Prefab\\TimeKeeper_BG.pref");
	SetSkillBGPrefab(pPrefab);

	Ptr<APrefab> pEffectPrefab = FIND(APrefab, L"Prefab\\TimeKeeperCookie_SkillBGEffect.pref");
	SetSkillBGEffectPrefab(pEffectPrefab);

	Ptr<APrefab> pAttackEffectPrefab = FIND(APrefab, L"Prefab\\TimeKeeper_SkillAttackEffect.pref");
	SetSKillEffectPrefab(pAttackEffectPrefab);

	m_TKBGEffectPrefab = FIND(APrefab, L"Prefab\\TimeKeeper_SkillEffect.pref");

	m_TKPostEffectPrefab = FIND(APrefab, L"Prefab\\Postprocess.pref");
}

// 스킬 해제 시 호출
void CTimeKeeperScript::OnUnequip()
{
	EndSkill();
}

void CTimeKeeperScript::Begin()
{
	CCookieSkillScript::Begin();
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_TKBGEffectPrefab, L"TKBGEffectPrefab");
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

	m_bEndReserved = false;

	EnterSkillMode();

	SpawnSkillBG(1);
	SpawnSkillBGEffect(1);
	//SpawnTKBGEffect(1);
	SpawnTKPostEffect(1);

	// 스킬 배경음악 재생
	SceneFlowMgr::GetInst()->PlayBGM(L"TimeKeeperCookie_Skill", 0.5f);

	// 스킬 시작 연출(1회)
	ChangeSkillState(ESkillState::Start);
}

void CTimeKeeperScript::EndSkill()
{
	if (!m_IsUsingSkill)
		return;

	m_IsUsingSkill = false;
	m_RemainDuration = 0.f;

	m_InitSkillPos = false;
	m_IsChargeMotion = false;

	m_ScoreTickAcc = 0.f;
	m_ScoreTickCount = 0;

	m_SkillState = ESkillState::None;
	m_bEndReserved = false;

	DestroySkillBG();
	//DestroySkillBGEffect();
	//DestroyTKBGEffect();
	DestroyTKPostEffect();
	//DestroySkillEffect();

	// 게임플레이 배경음악 복원
	SceneFlowMgr::GetInst()->PlayBGM(L"GamePlay_Sound", 0.5f);

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

	if (!m_bEndReserved)
	{
		m_RemainDuration -= DT;

		// 종료 예약
		if (m_RemainDuration <= 0.f)
		{
			m_RemainDuration = 0.f;
			m_bEndReserved = true;

			ChangeSkillState(ESkillState::End);
		}
	}

	if ( m_SkillState == ESkillState::Loop
		|| m_SkillState == ESkillState::Slide)
	{
		UpdateSkillMove();
		UpdateSkillScore();
	}

	// 슬라이드 이펙트 플립북 완료 감지 → 자동 제거
	if (m_SkillEffectObj != nullptr && !m_SkillEffectObj->IsDead())
	{
		if (m_SkillEffectObj->FlipbookRender() != nullptr
			&& m_SkillEffectObj->FlipbookRender()->IsAnimationComplete())
		{
			DestroySkillEffect();
		}
	}

	UpdateSkillAnimState();
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

void CTimeKeeperScript::TrySpawnSlideEffect()
{
	// 이미 이펙트가 살아있고 플립북이 재생 중이면 새로 생성하지 않음
	if (m_SkillEffectObj != nullptr && !m_SkillEffectObj->IsDead())
		return;

	SpawnSkillEffect(1);

	if (m_SkillEffectObj != nullptr)
	{
		// 플레이어보다 Z가 크면 깊이 테스트에서 플레이어 뒤쪽으로 배치됨
		Vec3 vPos = m_SkillEffectObj->Transform()->GetRelativePos();
		m_SkillEffectObj->Transform()->SetRelativePos(Vec3(vPos.x, vPos.y, 650.f));
	}
}

void CTimeKeeperScript::SpawnTKBGEffect(int _LayerIdx)
{
	if (m_TKBGEffectPrefab == nullptr)
		return;

 	DestroyTKBGEffect();

	m_TKBGEffectObject = m_TKBGEffectPrefab->Instantiate();
	CreateObject(m_TKBGEffectObject, _LayerIdx);
}

void CTimeKeeperScript::DestroyTKBGEffect()
{
	if (m_TKBGEffectObject == nullptr || m_TKBGEffectObject->IsDead())
	{
		m_TKBGEffectObject = nullptr;
		return;
	}

	TaskInfo info = {};
	info.Type    = TASK_TYPE::DESTROY_OBJECT;
	info.Param_0 = (DWORD_PTR)m_TKBGEffectObject;
	TaskMgr::GetInst()->AddTask(info);
	m_TKBGEffectObject = nullptr;
}

void CTimeKeeperScript::SpawnTKPostEffect(int _LayerIdx)
{
	if (m_TKPostEffectPrefab == nullptr)
		return;

	DestroyTKPostEffect();

	m_TKPostEffectObject = m_TKPostEffectPrefab->Instantiate();
	CreateObject(m_TKPostEffectObject, _LayerIdx);
}

void CTimeKeeperScript::DestroyTKPostEffect()
{
	if (m_TKPostEffectObject == nullptr || m_TKPostEffectObject->IsDead())
	{
		m_TKPostEffectObject = nullptr;
		return;
	}

	TaskInfo info = {};
	info.Type = TASK_TYPE::DESTROY_OBJECT;
	info.Param_0 = (DWORD_PTR)m_TKPostEffectObject;
	TaskMgr::GetInst()->AddTask(info);
	m_TKPostEffectObject = nullptr;
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

	// 월드 Y축 기준 고정 범위 제한
	if (vPos.y > m_MaxSkillY)
		vPos.y = m_MaxSkillY;

	if (vPos.y < m_MinSkillY)
		vPos.y = m_MinSkillY;

	pObj->Transform()->SetRelativePos(vPos);
}

void CTimeKeeperScript::UpdateSkillScore()
{
	if (m_SkillState != ESkillState::Slide)
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



// 스킬 애니메이션 상태 업데이트
void CTimeKeeperScript::UpdateSkillAnimState()
{
	bool bSlidePressed = KEY_PRESSED(KEY::DOWN);

	switch (m_SkillState)
	{
	case ESkillState::Start:
	{
		// 시작 연출이 끝나면
		// DOWN 누르고 있으면 Slide, 아니면 Loop
		if (GetOwner()->FlipbookRender()->IsAnimationComplete())
		{
			if (bSlidePressed)
				ChangeSkillState(ESkillState::Slide);
			else
				ChangeSkillState(ESkillState::Loop);
		}
	}
	break;

	case ESkillState::Loop:
	{
		// DOWN 홀드 시작 시 Slide 진입
		if (bSlidePressed)
		{
			ChangeSkillState(ESkillState::Slide);
		}
	}
	break;

	case ESkillState::Slide:
	{
		// DOWN 누르고 있는 동안 계속 Slide 유지
		// 떼면 다시 Loop
		if (!bSlidePressed)
		{
			ChangeSkillState(ESkillState::Loop);
		}
	}
	break;

	case ESkillState::End:
	{
		// 종료 연출 끝나면 스킬 완전 종료
		if (GetOwner()->FlipbookRender()->IsAnimationComplete())
		{
			EndSkill();
		}
	}
	break;

	default:
		break;
	}
}

void CTimeKeeperScript::ChangeSkillState(ESkillState _NextState)
{
	if (m_SkillState == _NextState)
		return;

	m_SkillState = _NextState;
	PlaySkillAnim(_NextState);

	if (m_Player == nullptr)
		return;

	switch (_NextState)
	{
	case ESkillState::Start:
		m_IsChargeMotion = false;
		m_Player->SetIsTimeKeeperSkillAnim2(false);
		break;

	case ESkillState::Loop:
		m_IsChargeMotion = false;
		m_Player->SetIsTimeKeeperSkillAnim2(false);
		break;

	case ESkillState::Slide:
		m_IsChargeMotion = true;
		m_Player->SetIsTimeKeeperSkillAnim2(true);
		TrySpawnSlideEffect();
		break;

	case ESkillState::End:
		m_IsChargeMotion = false;
		m_Player->SetIsTimeKeeperSkillAnim2(false);
		break;

	default:
		break;
	}
}

void CTimeKeeperScript::PlaySkillAnim(ESkillState _AnimState)
{
	if (GetOwner() == nullptr || GetOwner()->FlipbookRender() == nullptr)
		return;

	switch (_AnimState)
	{
	case ESkillState::Start:
		// 스킬 시작 Flipbook(1) : 1회 재생
		GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::Skill_1, 12.f, 0);
		break;

	case ESkillState::Loop:
		// 스킬 진행 Flipbook(2) : 반복 재생
		GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::Skill_2, 12.f, -1);
		break;

	case ESkillState::Slide:
		// 스킬 슬라이드 Flipbook(3) : DOWN 홀드 동안 유지되어야 하므로 반복 재생
		GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::Skill_3, 12.f, -1);
		break;

	case ESkillState::End:
		// 스킬 종료 Flipbook(4) : 1회 재생
		GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::Skill_4, 12.f, 0);
		break;

	default:
		break;
	}
}
