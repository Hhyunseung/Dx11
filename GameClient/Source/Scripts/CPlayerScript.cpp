#include "pch.h"
#include "CPlayerScript.h"

#include "AssetMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "LevelMgr.h"
#include "RenderMgr.h"
#include "TaskMgr.h"
#include "GamePlayMgr.h"
#include "CStateMachine.h"

#include "GameObject.h"

#include "CRunState.h"
#include "CJumpState.h"
#include "CDoubleJumpState.h"
#include "CLandState.h"
#include "CSlideState.h"
#include "CHitState.h"
#include "CFallState.h"
#include "CDeadState.h"

#include "CMovingPlatformScirpt.h"
#include "CCookieSkillScript.h"

#include "CGamePlayUIScript.h"
#include "CJumpButtonScript.h"
#include "CSlideButtonScript.h"
#include "CHPBarScript.h"
#include "CHitEffectScript.h"

CPlayerScript::CPlayerScript()
	: CScript(SCRIPT_TYPE::PLAYERSCRIPT)
	, m_FeetCollider(nullptr)
	, m_FeetObject(nullptr)
	, m_CurrentMovingPlatform(nullptr)
	, m_CookieSkill(nullptr)
	, m_MaxHP(100)
	, m_CurrentHP(100)
	, m_Damage(10)
	, m_FallDamage(20)
	, m_PrevFeetY(0.f)
	, m_CurFeetY(0.f)
	, m_gravity(-980.f)
	, m_VelY(0.f)
	, m_JumpPower(800.f)
	, m_DoubleJumpPower(600.f)
	, m_BlockFall(false)
	, m_InvincibleTime(2.f)
	, m_InvincibleTimer(0.f)
	, m_BlinkTime(0.2f)
	, m_InvincibleFloorLine(-300.f)
	, m_FallDeadLine(-700.f)
	, m_FallRescueSpeed(500.f)
	, m_FallRescueInvincibleDuration(3.f)
	, m_HitInvincibleDuration(2.f)
	, m_JumpCount(0)
	, m_MaxJumpCount(2)
	, m_IsSkillMoveMode(false)
	, m_IsTimeKeeperSkillAnim2(false)
	, m_JumpRequest(false)
	, m_IsLand(false)
	, m_IsJump(false)
	, m_IsDoubleJump(false)
	, m_IsSlide(false)
	, m_IsInvincible(false)
	, m_IsFallRescue(false)
	, m_DeathPending(false)
	, m_IsDead(false)
	, m_IsGiant(false)
	, m_GiantTimer(0.f)
	, m_GiantDuration(5.f)
	, m_DefaultPlayerScale(300.f)
	, m_GiantTargetScale(600.f)
	, m_GiantScaleSpeed(300.f)
	, m_IsReturningFromGiant(false)
	, m_IsBoost(false)
	, m_BoostTimer(0.f)
	, m_BoostDuration(5.f)
	, m_DefaultScrollSpeed(300.f)
	, m_DashScrollSpeed(500.f)
	, m_BoostEffectPrefab(nullptr)
	, m_BoostEffectObject(nullptr)
	, m_IsMagnet(false)
	, m_MagnetTimer(0.f)
	, m_MagnetDuration(5.f)
	, m_MagnetRange(500.f)
{

}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Init()
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpPower, L"JumpPower", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::PREFAB, &m_Missile, L"Missile", true, 0.f);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, &m_Tex, L"ChangeTex");
}


void CPlayerScript::Begin()
{
	m_StateMachine = new CStateMachine;

	// 상태 등록
	m_StateMachine->AddState(new CRunState(this));
	m_StateMachine->AddState(new CJumpState(this));
	m_StateMachine->AddState(new CDoubleJumpState(this));
	m_StateMachine->AddState(new CLandState(this));
	m_StateMachine->AddState(new CSlideState(this));
	m_StateMachine->AddState(new CHitState(this));
	m_StateMachine->AddState(new CFallState(this));
	m_StateMachine->AddState(new CDeadState(this));

	// 시작 시에는 일단 공중으로 가정
	m_IsLand = false;
	m_JumpCount = 0;
	m_BlockFall = false;
	m_VelY = 0.f;

	m_StateMachine->StartState(PLAYER_STATE_ID::JUMP);

	// GamePlayMgr에 플레이어 등록
	GamePlayMgr::GetInst()->SetPlayerObject(GetOwner());
	GamePlayMgr::GetInst()->SetPlayerScript(this);

	// 플레이어에게 스킬 적용 (임시)
	GamePlayMgr::GetInst()->ApplySkillToPlayer();


	GetOwner()->SetLayerIdx(3); // Player 레이어

	Ptr<GameObject> pChild = GetOwner()->GetChild(1);
	m_FeetObject = pChild.Get();
	m_FeetObject->SetLayerIdx(4); // PlayerFeet 레이어
	m_FeetCollider = m_FeetObject->Collider2D().Get();

	
	m_FeetCollider->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CPlayerScript::FeetBeginOverlap);
	m_FeetCollider->AddDynamicOverlap(this, (COLLISION_EVENT)&CPlayerScript::FeetOverlap);
	m_FeetCollider->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CPlayerScript::FeetEndOverlap);

	GetOwner()->Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CPlayerScript::BeginOverlap);

	// 부스트 이펙트 프리팹 로드
	m_BoostEffectPrefab = LOAD(APrefab, L"Prefab\\Effect_Boost.pref");

	// 임시
	// 플레이어 전용 머티리얼 생성
	FlipbookRender()->CreateDynamicMaterial();
	// 초기 알파는 0 (셰이더에서 적용 안 함)
	FlipbookRender()->GetMaterial()->SetScalar(FLOAT_0, 0.f);

	m_CurrentHP = m_MaxHP;

	InitFeetColliderShape();
	SetDefaultCollider();

	UpdateHPUI();
}

void CPlayerScript::Tick()
{
	if (GamePlayMgr::GetInst()->IsPaused())
		return;


	if (m_IsDead)
	{
 		if (m_StateMachine != nullptr)
			m_StateMachine->Tick();
		return;
	}

	if (m_IsFallRescue)
	{
		UpdateFallRescue();
		
		if (m_StateMachine != nullptr)
			m_StateMachine->Tick();
		return;
	}

	m_PrevFeetY = GetOwner()->Collider2D()->GetBottomY();

	ApllyMovingPlatform();
	ApplyCurrentColliderState();
	UpdateAutoHPDecrease();

	UpdateGiantMode();
	UpdateItemBuffs();

	if (!m_IsSkillMoveMode)
	{
		HandleJump();
		HandleSlide();
		UpdateUIButton();

		ProcessJump();
		GravityAndMove();
		
		ClampDuringInvincibility();

		CheckFallOut(); // 낙하 체크
		UpdateInvincibility();  // 매 프레임 무적 타이머 업데이트

		if (m_StateMachine != nullptr)
			m_StateMachine->Tick();

		if (!m_IsDead && m_DeathPending && m_IsLand)
		{
			Die();
			return;
		}
	}
	else
	{
		// 시간지기 스킬 활성 상태
		// 일반 점프/슬라이드/중력 처리 막기
		// 상하 이동은 CTimeKeeperSkillScript가 처리
		UpdateUIButton();
		UpdateInvincibility();
	}

	m_CurFeetY = GetOwner()->Collider2D()->GetBottomY();
}

// 점프 입력 처리
void CPlayerScript::HandleJump()
{
	bool bKeyboardJump = KEY_TAP(KEY::SPACE);
	bool bUIJump = false;

	CGamePlayUIScript* pUI = GamePlayMgr::GetInst()->GetGamePlayUIScript();
	if (pUI != nullptr && pUI->GetJumpButton() != nullptr)
	{
		bUIJump = pUI->GetJumpButton()->ConsumeClick();
	}


	if (bKeyboardJump || bUIJump)
	{
		m_JumpRequest = true;
	}
}

void CPlayerScript::HandleSlide()
{
	bool bKeyboardSlide = KEY_PRESSED(KEY::DOWN);
	bool bUISlide = false;

	CGamePlayUIScript* pUI = GamePlayMgr::GetInst()->GetGamePlayUIScript();
	if (pUI != nullptr && pUI->GetSlideButton() != nullptr)
	{
		bUISlide = pUI->GetSlideButton()->GetPressed();
	}

	bool bSlideHeld = (bKeyboardSlide || bUISlide);

	// =========================
	// giant 상태에서는 콜라이더 절대 안 바꿈
	// =========================
	if (m_IsGiant || m_IsReturningFromGiant)
	{
		if (bSlideHeld)
		{
			if (m_IsLand && !m_IsSlide)
			{
				m_IsSlide = true;
				m_LastSafePos = GetOwner()->Transform()->GetRelativePos();
				ChangeState(PLAYER_STATE_ID::SLIDE);
			}
		}
		else
		{
			if (m_IsSlide)
			{
				m_IsSlide = false;

				if (m_IsLand)
				{
					m_LastSafePos = GetOwner()->Transform()->GetRelativePos();
					ChangeState(PLAYER_STATE_ID::RUN);
				}
			}
		}

		return;
	}

	// =========================
	// 일반 상태일 때만 collider 변경
	// =========================
	if (bSlideHeld)
	{
		if (m_IsLand && !m_IsSlide)
		{
			m_IsSlide = true;
			m_LastSafePos = GetOwner()->Transform()->GetRelativePos();
			ChangeState(PLAYER_STATE_ID::SLIDE);
		}
	}
	else
	{
		if (m_IsSlide)
		{
			m_IsSlide = false;

			if (m_IsLand)
			{
				m_LastSafePos = GetOwner()->Transform()->GetRelativePos();
				ChangeState(PLAYER_STATE_ID::RUN);
			}
		}
	}
}

void CPlayerScript::HandleHit()
{
}

void CPlayerScript::UpdateAutoHPDecrease()
{
	if (m_IsDead || m_DeathPending)
		return;

   if (m_CurrentHP <= 0)
	{
		m_CurrentHP = 0;
		UpdateHPUI();
		RequestDie();
		return;
	}

	m_AutoHPDecreaseTimer += DT;

	if (m_AutoHPDecreaseTimer >= 1.f)
	{
		m_AutoHPDecreaseTimer = 0.f;

		m_CurrentHP--;

		if (m_CurrentHP < 0)
			m_CurrentHP = 0;

		UpdateHPUI();
	}
}

// 점프처리 (중력과 별개로 점프 입력이 들어왔을 때 수직 속도 설정)
void CPlayerScript::ProcessJump()
{
	if (!m_JumpRequest)
		return;

	// 착지 상태에서 점프
	if (m_JumpCount == 0)
	{
		m_IsLand = false;
		m_IsJump = true;
		m_IsDoubleJump = false;

		m_VelY = m_JumpPower;
		m_JumpCount = 1;

		m_CurrentMovingPlatform = nullptr;
		ChangeState(PLAYER_STATE_ID::JUMP);
	}
	// 더블 점프
	else if (m_JumpCount == 1)
	{
		m_IsLand = false;
		m_IsJump = false;
		m_IsDoubleJump = true;

		m_VelY = m_DoubleJumpPower;
		m_JumpCount = 2;

		m_CurrentMovingPlatform = nullptr;
		ChangeState(PLAYER_STATE_ID::DOUBLE_JUMP);
	}

	m_JumpRequest = false;
}



void CPlayerScript::UpdateUIButton()
{
	CGamePlayUIScript* pUI = GamePlayMgr::GetInst()->GetGamePlayUIScript();
	if (pUI == nullptr)
		return;

	if (pUI->GetJumpButton() != nullptr)
	{
		bool bJumpHeld = KEY_PRESSED(KEY::SPACE)
			|| pUI->GetJumpButton()->IsMouseHolding();

		pUI->GetJumpButton()->SetPressed(bJumpHeld);
	}

	if (pUI->GetSlideButton() != nullptr)
	{
		bool bSlideHeld = KEY_PRESSED(KEY::DOWN)
			|| pUI->GetSlideButton()->IsMouseHolding();

		pUI->GetSlideButton()->SetPressed(bSlideHeld);
	}
}

// 플랫폼 이동 적용
void CPlayerScript::ApllyMovingPlatform()
{
	if (nullptr == m_CurrentMovingPlatform)
		return;

	Vec3 vPos = Transform()->GetRelativePos();
	//vPos += m_CurrentMovingPlatform->GetFrameDelta();
	Transform()->SetRelativePos(vPos);
}

// 중력 적용 및 이동 처리
void CPlayerScript::GravityAndMove()
{
	if (m_IsLand)
		return;

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	m_VelY += m_gravity * 2 * DT;
	vPos.y += m_VelY * DT;

	GetOwner()->Transform()->SetRelativePos(vPos);
}

void CPlayerScript::UpdateInvincibility()
{
	if (!m_IsInvincible)
		return;

	m_InvincibleTimer += DT;

    // 깜빡임 알파값
	// m_BlinkTime초 주기로 0.5와 1을 오가도록 설정 (토글)
	int phase = (int)(m_InvincibleTimer / m_BlinkTime) % 2; // 0 또는 1
	float blinkAlpha = (phase == 0) ? 0.8f : 0.6f;
	FlipbookRender()->GetMaterial()->SetScalar(FLOAT_0, blinkAlpha);

	if (m_InvincibleTimer >= m_InvincibleTime)
	{
		m_IsInvincible = false;
		m_InvincibleTimer = 0.f;
		m_BlockFall = false;
		// 알파값 초기화 (0이면 셰이더에서 적용 안 함)
		FlipbookRender()->GetMaterial()->SetScalar(FLOAT_0, 0.f);
	}
}

void CPlayerScript::StartInvincibility(float _Duration)
{
	m_IsInvincible = true;
	m_BlockFall = true;
	m_InvincibleTime = _Duration;
	m_InvincibleTimer = 0.f;
}

void CPlayerScript::ClampDuringInvincibility()
{
	if (!m_BlockFall)
		return;

	if (m_IsDead || m_IsFallRescue)
		return;

	float bottomY = GetOwner()->Collider2D()->GetBottomY();

	if (bottomY >= m_InvincibleFloorLine)
		return;

	Vec3 vPos = Transform()->GetRelativePos();

	float offset = m_InvincibleFloorLine - bottomY;
	vPos.y += offset;

	Transform()->SetRelativePos(vPos);

	if (m_VelY < 0.f)
		m_VelY = 0.f;
}

// 낙하 체크
void CPlayerScript::CheckFallOut()
{
	if (m_IsDead || m_IsFallRescue)
		return;

	if (m_IsInvincible)
		return;

	float bottomY = GetOwner()->Collider2D()->GetBottomY();

	if (bottomY < m_FallDeadLine) // 낙사 기준 y 좌표
	{
		// 낙하 구출 시작
		BeginFallRescue();
	}
}

// 낙하 구출 시작
void CPlayerScript::BeginFallRescue()
{
	m_IsFallRescue = true;
	m_IsSkillMoveMode = false;
	m_JumpRequest = false;
	m_IsJump = false;
	m_IsDoubleJump = false;
	m_IsSlide = false;
	m_IsLand = false;
	m_CurrentMovingPlatform = nullptr;
	m_GroundColliders.clear();

	m_CurrentHP -= m_FallDamage;
	if (m_CurrentHP < 0)
		m_CurrentHP = 0;

	UpdateHPUI();


	if (m_CurrentHP <= 0)
	{
		m_CurrentHP = 0;
		UpdateHPUI();
		return;
	}

	ChangeState(PLAYER_STATE_ID::FALL);

}

// 낙하 구출 업데이트 (낙사 구출 중일 때 매 프레임 호출)
void CPlayerScript::UpdateFallRescue()
{
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	
	Vec3 target = m_LastSafePos + Vec3(0.f, 200.f, 0.f); // 안전 위치보다 약간 위로 이동

	Vec3 dir = target - pos;
	float length = dir.Length();

	if (length < 5.f)
	{
		Transform()->SetRelativePos(target);

		m_IsFallRescue = false;
		m_VelY = 0.f;

		StartInvincibility(m_FallRescueInvincibleDuration); // 구출 후 무적

		ChangeState(PLAYER_STATE_ID::JUMP);
		return;
	}

	dir.Normalize();
	pos += dir * m_FallRescueSpeed * DT; // 낙하 구출 이동 속도
	Transform()->SetRelativePos(pos);
}

void CPlayerScript::Die()
{
	if (m_IsDead)
		return;

	if (!m_IsLand)
		return;

	m_IsDead = true;
	m_DeathPending = false;

	m_IsInvincible = false;
	m_IsFallRescue = false;
	m_BlockFall = false;
	m_IsSkillMoveMode = false;

	m_JumpRequest = false;
	m_IsJump = false;
	m_IsDoubleJump = false;
	m_IsSlide = false;

	m_VelY = 0.f;

	GamePlayMgr::GetInst()->SetScrollSpeed(0.f); // 스크롤 멈춤

	ChangeState(PLAYER_STATE_ID::DEAD);
}

void CPlayerScript::RequestDie()
{
	if (m_IsDead || m_DeathPending)
		return;

	if (m_IsLand)
	{
		Die();
	}
	else
	{
		m_DeathPending = true;

		// 공중에서 죽음 예약되면 더 이상 일반 피격/구출/스킬 상태가 꼬이지 않게 정리
		m_IsInvincible = false;
		m_IsFallRescue = false;
		m_BlockFall = false;
		m_IsSkillMoveMode = false;

		m_JumpRequest = false;
		m_IsJump = false;
		m_IsDoubleJump = false;
		m_IsSlide = false;
	}
}


void CPlayerScript::UpdateItemBuffs()
{
	//if (m_IsGiant)
	//{
	//	m_GiantTimer += DT;
	//	if (m_GiantTimer >= m_GiantDuration)
	//	{
	//		m_IsGiant = false;
	//		m_GiantTimer = 0.f;

	//		Transform()->SetRelativeScale(Vec3(m_DefaultPlayerScale, m_DefaultPlayerScale, 1.f));
	//	}
	//}

	if (m_IsBoost)
	{
		UpdateBoostEffect();

		m_BoostTimer += DT;
		if (m_BoostTimer >= m_BoostDuration)
		{
			m_IsBoost = false;
			m_BoostTimer = 0.f;
			GamePlayMgr::GetInst()->SetScrollSpeed(m_DefaultScrollSpeed);

			DestroyBoostEffect();
		}
	}

	if (m_IsMagnet)
	{
		m_MagnetTimer += DT;
		if (m_MagnetTimer >= m_MagnetDuration)
		{
			m_IsMagnet = false;
			m_MagnetTimer = 0.f;
		}
	}
}

void CPlayerScript::UpdateGiantMode()
{
	float curScale = Transform()->GetRelativeScale().x;

	if (m_IsGiant)
	{
		m_GiantTimer += DT;

		if (curScale < m_GiantTargetScale)
			UpdateGiantScaleUp();

		if (m_GiantTimer >= m_GiantDuration)
		{
			m_IsGiant = false;
			m_IsReturningFromGiant = true;
			m_GiantTimer = 0.f;
		}
	}
	else if (m_IsReturningFromGiant)
	{
		UpdateGiantScaleUp();

		float newScale = Transform()->GetRelativeScale().x;
		if (fabs(newScale - m_DefaultPlayerScale) <= 0.5f)
		{
			m_IsReturningFromGiant = false;

			float prevBottomY = GetOwner()->Collider2D()->GetBottomY();

			Transform()->SetRelativeScale(Vec3(m_DefaultPlayerScale, m_DefaultPlayerScale, 1.f));
			KeepBottomAligned(prevBottomY);
		}
	}
}

void CPlayerScript::UpdateGiantScaleUp()
{
	Ptr<CCollider2D> pBodyCol = GetOwner()->Collider2D();
	if (pBodyCol == nullptr)
		return;

	float prevBottomY = pBodyCol->GetBottomY();

	float curScale = Transform()->GetRelativeScale().x;
	float targetScale = m_IsGiant ? m_GiantTargetScale : m_DefaultPlayerScale;

	float nextScale = curScale;

	if (curScale < targetScale)
	{
		nextScale += m_GiantScaleSpeed * DT;
		if (nextScale > targetScale)
			nextScale = targetScale;
	}
	else if (curScale > targetScale)
	{
		nextScale -= m_GiantScaleSpeed * DT;
		if (nextScale < targetScale)
			nextScale = targetScale;
	}

	if (fabs(nextScale - curScale) <= 0.01f)
		return;

	Transform()->SetRelativeScale(Vec3(nextScale, nextScale, 1.f));

	KeepBottomAligned(prevBottomY);
}

void CPlayerScript::InitFeetColliderShape()
{
	if (m_FeetCollider == nullptr)
		return;

	// 발 콜라이더 자체 크기는 고정
	m_FeetCollider->SetScale(Vec2(1.f, 1.f));
	m_FeetCollider->SetOffset(Vec2(0.f, 0.f));
}

void CPlayerScript::SetDefaultFeetTransform()
{
	if (m_FeetObject == nullptr)
		return;

	// 기본 발 위치
	m_FeetObject->Transform()->SetRelativePos(Vec3(-5.f, -132.f, 0.f));
	m_FeetObject->Transform()->SetRelativeScale(Vec3(60.f, 7.f, 0.f));
}

void CPlayerScript::SetGiantFeetTransform()
{
	if (m_FeetObject == nullptr)
		return;

	// giant 상태에서 살짝만 아래로
	// 필요하면 -145.f / -150.f 식으로 미세 조정
	m_FeetObject->Transform()->SetRelativePos(Vec3(-5.f, -255.f, 0.f));
	m_FeetObject->Transform()->SetRelativeScale(Vec3(150.f, 10.f, 0.f));
}

void CPlayerScript::SetSlideFeetTransform()
{
	if (m_FeetObject == nullptr)
		return;

	// 슬라이드는 기본과 동일하게 두고 시작
	m_FeetObject->Transform()->SetRelativePos(Vec3(0.f, -135.f, 0.f));
}

void CPlayerScript::KeepBottomAligned(float _PrevBottomY)
{
	Ptr<CCollider2D> pBodyCol = GetOwner()->Collider2D();
	if (pBodyCol == nullptr)
		return;

	float newBottomY = pBodyCol->GetBottomY();
	float deltaY = _PrevBottomY - newBottomY;

	Vec3 vPos = Transform()->GetRelativePos();
	vPos.y += deltaY;
	Transform()->SetRelativePos(vPos);

	if (m_VelY < 0.f)
		m_VelY = 0.f;
}

void CPlayerScript::ApplyCurrentColliderState()
{
	if (m_IsGiant || m_IsReturningFromGiant)
	{
		SetGiantCollider();
		return;
	}

	if (m_IsSlide)
	{
		SetSlideCollider();
		return;
	}

	SetDefaultCollider();
}

void CPlayerScript::UpdateHPUI()
{
	CGamePlayUIScript* pUI = GamePlayMgr::GetInst()->GetGamePlayUIScript();

	if (pUI == nullptr)
		return;

	if (pUI->GetHPBar() == nullptr)
		return;

	pUI->GetHPBar()->SetHP(m_CurrentHP, m_MaxHP);
}

bool CPlayerScript::HasGroundCollider(CCollider2D* _Collider)
{
	if (_Collider == nullptr)
		return false;

	for (CCollider2D* pCol : m_GroundColliders)
	{
		if (pCol == _Collider)
			return true;
	}

	return false;
}

void CPlayerScript::SetIsSkillMoveMode(bool _Value)
{
	if (m_IsSkillMoveMode == _Value)
		return;

	m_IsSkillMoveMode = _Value;

	if (m_IsSkillMoveMode)
	{
		// 스킬 이동 모드 진입 시 처리
		m_JumpRequest = false;
		m_IsSlide = false;
		m_IsJump = false;
		m_IsDoubleJump = false;
		m_VelY = 0.f;
		m_JumpCount = 0;

		ApplyCurrentColliderState();
	}
	else
	{
		// 스킬 이동 모드 종료 시 처리
		m_IsSlide = false;
		m_IsTimeKeeperSkillAnim2 = false;

		ApplyCurrentColliderState();

		// 현재 바닥 접촉 여부 재판단
		if (m_GroundColliders.empty())
		{
			// 공중이면 즉시 낙하 시작
			m_IsLand = false;
			m_VelY = 0.f;
		}
		else
		{
			// 바닥 위면 RUN 복귀
			m_IsLand = true;
			m_VelY = 0.f;
			m_JumpCount = 0;

			if (m_StateMachine != nullptr)
			{
				ChangeState(PLAYER_STATE_ID::RUN);
			}
		}
	}
}

void CPlayerScript::TakeDamage(int _Damage)
{
	if (m_IsDead || m_DeathPending)
		return;

	// 무적 상태면 무시
	if (m_IsInvincible) 
		return;

	m_CurrentHP -= _Damage;
	if (m_CurrentHP < 0)
		m_CurrentHP = 0;

 	int HP = m_CurrentHP;

	UpdateHPUI();

	// Hit Effect UI 표시 (전체화면 빨간 테두리)
	CGamePlayUIScript* pUI = GamePlayMgr::GetInst()->GetGamePlayUIScript();
	if (pUI != nullptr && pUI->GetHitEffect() != nullptr)
		pUI->GetHitEffect()->Trigger();

	// 사망 체크
	if (m_CurrentHP <= 0)
	{
		m_CurrentHP = 0;
		UpdateHPUI();
		RequestDie();
		return;
	}

	StartInvincibility(m_HitInvincibleDuration); // 피격 후 무적
	ChangeState(PLAYER_STATE_ID::HIT);
}

void CPlayerScript::Heal(int _Amount)
{
	if (_Amount <= 0)
		return;

	m_CurrentHP += _Amount;

	if (m_CurrentHP > m_MaxHP)
		m_CurrentHP = m_MaxHP;

	UpdateHPUI();
}


// ================== 아이템 효과 활성화 함수들 ==================

void CPlayerScript::ActivateGiant(float _Duration)
{
	m_IsGiant = true;
	m_IsReturningFromGiant = false;
	m_GiantTimer = 0.f;
	m_GiantDuration = _Duration;

	float prevBottomY = GetOwner()->Collider2D()->GetBottomY();
	KeepBottomAligned(prevBottomY);
}

void CPlayerScript::ActivateBoost(float _Duration)
{
	m_IsBoost = true;
	m_BoostTimer = 0.f;
	m_BoostDuration = _Duration;

	m_DefaultScrollSpeed = GamePlayMgr::GetInst()->GetScrollSpeed();
	GamePlayMgr::GetInst()->SetScrollSpeed(m_DashScrollSpeed);

	SpawnBoostEffect(1); // 레이어 1 → 플레이어(레이어 3)보다 먼저 렌더링 → 캐릭터 뒤에 표시
}

void CPlayerScript::ActivateMagnet(float _Duration)
{
	m_IsMagnet = true;
	m_MagnetTimer = 0.f;
	m_MagnetDuration = _Duration;
}

void CPlayerScript::SpawnBoostEffect(int _LayerIdx)
{
	if (m_BoostEffectPrefab == nullptr)
		return;

	DestroyBoostEffect();

	m_BoostEffectObject = m_BoostEffectPrefab->Instantiate();

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	vPos.x -= 90.f;
	vPos.y -= 60.f; 
	vPos.z += 1.f;
	m_BoostEffectObject->Transform()->SetRelativePos(vPos);

	GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::BOOST_RUN, 12.f, -1);

	CreateObject(m_BoostEffectObject, _LayerIdx);
}

void CPlayerScript::DestroyBoostEffect()
{
	if (m_BoostEffectObject == nullptr || m_BoostEffectObject->IsDead())
	{
		m_BoostEffectObject = nullptr;
		return;
	}

	TaskInfo info = {};
	info.Type    = TASK_TYPE::DESTROY_OBJECT;
	info.Param_0 = (DWORD_PTR)m_BoostEffectObject;
	TaskMgr::GetInst()->AddTask(info);
	m_BoostEffectObject = nullptr;
}

void CPlayerScript::UpdateBoostEffect()
{
	if (m_BoostEffectObject == nullptr || m_BoostEffectObject->IsDead())
	{
		m_BoostEffectObject = nullptr;
		return;
	}

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	vPos.x -= 90.f;
	vPos.y -= 60.f;
	vPos.z += 1.f;
	m_BoostEffectObject->Transform()->SetRelativePos(vPos);
}


//void CPlayerScript::UpdateSkillAnimationState()
//{
//	//if (KEY_TAP(KEY::SPACE))
//	//{
//	//	Vec3 vMyPos = Transform()->GetRelativePos();
//	//	Vec3 vMyScale = Transform()->GetRelativeScale();
//	//	Vec3 vRotation = Transform()->GetRelativeRot();
//	//	Vec3 vUp = Transform()->GetDir(DIR::UP);
//
//	//	//Instantiate(m_Missile.Get(), 4, vMyPos + vMyScale * 0.5f * vUp);
//
//	//	//LevelMgr::GetInst()->GetCurrentLevel()->AddObject(0, pObject);
//	//} 
//
//	//if (m_IsTimeKeeperSkillAnim2)
//	//{
//	//	GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::Skill_2, 12.f, 1);
//	//}
//	//else
//	//{
//	//	GetOwner()->FlipbookRender()->Play((UINT)PLAYER_STATE_ID::Skill_3, 12.f, -1);
//	//}
//}

void CPlayerScript::Slide()
{
}

void CPlayerScript::SetDefaultCollider()
{
	GetOwner()->Collider2D()->SetOffset(Vec2(-0.02f, -0.26f));
	GetOwner()->Collider2D()->SetScale(Vec2(0.23f, 0.34f));

	SetDefaultFeetTransform();
}

void CPlayerScript::SetSlideCollider()
{
	GetOwner()->Collider2D()->SetOffset(Vec2(-0.02f, -0.34f));
	GetOwner()->Collider2D()->SetScale(Vec2(0.23f, 0.17f));
}

void CPlayerScript::SetGiantCollider()
{
	GetOwner()->Collider2D()->SetScale(Vec2(0.24f, 0.36f));
	GetOwner()->Collider2D()->SetOffset(Vec2(-0.02f, -0.20f));

	SetGiantFeetTransform();
}

// 충돌 체크용 충돌체
void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}



// 땅 체크용 충돌체
void CPlayerScript::FeetBeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (_OtherCollider == nullptr)
		return;

	// 목록에 없으면 추가
	if (!HasGroundCollider(_OtherCollider))
	{
		m_GroundColliders.push_back(_OtherCollider);
	}

	// 이동 플랫폼 위에 올라탄 경우, 현재 타고 있는 플랫폼 정보 업데이트
	GameObject* pOtherObj = _OtherCollider->GetOwner();
	if (pOtherObj != nullptr)
	{
		CMovingPlatformScirpt* pPlatform = pOtherObj->GetScript<CMovingPlatformScirpt>().Get();
		if (pPlatform != nullptr)
		{
			m_CurrentMovingPlatform = pPlatform;
		}
	}
}

void CPlayerScript::FeetOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (_OtherCollider == nullptr)
		return;

	const float platformTopY = _OtherCollider->GetTopY();
	const float feetBottomY = m_FeetCollider->GetBottomY();

	float prevFeetY = m_PrevFeetY;
	float curFeetY = m_FeetCollider->GetBottomY(); // 여기서 직접 가져옴

	if (!m_IsSkillMoveMode
		&& !m_IsLand
		&& m_VelY <= 0.f // 내려오는 중인지 체크
		&& feetBottomY <= platformTopY
		&& m_PrevFeetY >= platformTopY
		&& curFeetY <= platformTopY)
	{
		// 플레이어 위치를 플랫폼 위로 보정
		Vec3 playerPos = GetOwner()->Transform()->GetRelativePos();

		float offsetY = platformTopY - feetBottomY;
		playerPos.y += offsetY;

		GetOwner()->Transform()->SetRelativePos(playerPos);

		// 착지 상태 갱신
		m_IsLand = true;
		m_JumpCount = 0;
		m_VelY = 0.f;

		if (m_CurrentHP <= 0 || m_DeathPending)
		{
			m_DeathPending = true;
			return;
		}

		// 현재 닿은 바닥이 moving platform이면 저장
		GameObject* pOtherObj = _OtherCollider->GetOwner();
		if (pOtherObj != nullptr)
		{
			m_CurrentMovingPlatform = pOtherObj->GetScript<CMovingPlatformScirpt>().Get();
		}
		else
		{
			m_CurrentMovingPlatform = nullptr;
		}

		if (m_StateMachine != nullptr)
			m_StateMachine->ChangeState(PLAYER_STATE_ID::LAND);
	}
}

void CPlayerScript::FeetEndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if (_OtherCollider == nullptr)
		return;


	for (auto iter = m_GroundColliders.begin(); iter != m_GroundColliders.end();)
	{
		if (*iter == _OtherCollider)
			iter = m_GroundColliders.erase(iter);
		else
			++iter;
	}

	GameObject* pOtherObj = _OtherCollider->GetOwner();
	if (pOtherObj != nullptr)
	{
		CMovingPlatformScirpt* pPlatform = pOtherObj->GetScript<CMovingPlatformScirpt>().Get();
		if (pPlatform != nullptr && m_CurrentMovingPlatform == pPlatform)
		{
			m_CurrentMovingPlatform = nullptr;
		}
	}

	if (m_GroundColliders.empty())
		m_IsLand = false;
}

void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_gravity, sizeof(float), 1, _File);
	fwrite(&m_JumpPower, sizeof(float), 1, _File);
	fwrite(&m_DoubleJumpPower, sizeof(float), 1, _File);
	fwrite(&m_IsLand, sizeof(bool), 1, _File);
	fwrite(&m_IsJump, sizeof(bool), 1, _File);
	fwrite(&m_IsDoubleJump, sizeof(bool), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_gravity, sizeof(float), 1, _File);
	fread(&m_JumpPower, sizeof(float), 1, _File);
	fread(&m_DoubleJumpPower, sizeof(float), 1, _File);
	fread(&m_IsLand, sizeof(bool), 1, _File);
	fread(&m_IsJump, sizeof(bool), 1, _File);
	fread(&m_IsDoubleJump, sizeof(bool), 1, _File);
}
