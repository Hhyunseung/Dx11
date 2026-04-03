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

#include "CMovingPlatformScirpt.h"

#include "CGamePlayUIScript.h"
#include "CJumpButtonScript.h"
#include "CSlideButtonScript.h"

CPlayerScript::CPlayerScript()
	: CScript(SCRIPT_TYPE::PLAYERSCRIPT)
	, m_FeetCollider(nullptr)
	, m_CurrentMovingPlatform(nullptr)
	, m_HP(100)
	, m_CurrentHP(100)
	, m_Damage(10)
	, m_PrevFeetY(0.f)
	, m_CurFeetY(0.f)
	, m_gravity(-980.f)
	, m_VelY(0.f)
	, m_JumpPower(800.f)
	, m_DoubleJumpPower(600.f)
	, m_InvincibleTime(2.f)
	, m_InvincibleTimer(0.f)
	, m_BlinkTime(0.2f)
	, m_JumpCount(0)
	, m_MaxJumpCount(2)
	, m_JumpRequest(false)
	, m_IsLand(false)
	, m_IsJump(false)
	, m_IsDoubleJump(false)
	, m_IsSlide(false)
	, m_IsInvincible(false)
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

	m_StateMachine->StartState(PLAYER_STATE_ID::RUN);

	// GamePlayMgr에 플레이어 등록
	GamePlayMgr::GetInst()->SetPlayerObject(GetOwner());
	GamePlayMgr::GetInst()->SetPlayerScript(this);

	GetOwner()->SetLayerIdx(3); // Player 레이어

	Ptr<GameObject> pChild = nullptr;
	
	pChild = GetOwner()->GetChild(1);
	pChild->SetLayerIdx(4); // PlayerFeet 레이어
	m_FeetCollider = pChild->Collider2D().Get();

	
	m_FeetCollider->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CPlayerScript::FeetBeginOverlap);
	m_FeetCollider->AddDynamicOverlap(this, (COLLISION_EVENT)&CPlayerScript::FeetOverlap);
	m_FeetCollider->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CPlayerScript::FeetEndOverlap);

	GetOwner()->Collider2D()->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CPlayerScript::BeginOverlap);


	// 임시
	// 플레이어 전용 머티리얼 생성
	FlipbookRender()->CreateDynamicMaterial();
	// 초기 알파는 0 (셰이더에서 적용 안 함)
	FlipbookRender()->GetMaterial()->SetScalar(FLOAT_0, 0.f);

	m_CurrentHP = 100;
}

void CPlayerScript::Tick()
{
	//m_PrevFeetY = GetOwner()->Transform()->GetRelativePos().y;
	m_PrevFeetY = GetOwner()->Collider2D()->GetBottomY();

	ApllyMovingPlatform();

	HandleJump();
	HandleSlide();
	UpdateUIButton();

	ProcessJump();

	GravityAndMove();
	UpdateInvincibility();  // 매 프레임 무적 타이머 업데이트

	m_StateMachine->Tick();
	Skill();

	//m_CurFeetY = GetOwner()->Transform()->GetRelativePos().y;
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

// 슬라이드 입력 처리
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

	if (bSlideHeld)
	{
		if (m_IsLand)
		{
			m_IsSlide = true;
			ChangeState(PLAYER_STATE_ID::SLIDE);
			SetSlideCollider();
		}
	}
	else
	{
		m_IsSlide = false;

		if (m_IsLand)
		{
			ChangeState(PLAYER_STATE_ID::RUN);
			SetDefaultCollider();
		}
	}
}

void CPlayerScript::HandleHit()
{
}

// 점프처리 (중력과 별개로 점프 입력이 들어왔을 때 수직 속도 설정)
void CPlayerScript::ProcessJump()
{
	if (!m_JumpRequest)
		return;

	SetDefaultCollider();

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
		// 알파값 초기화 (0이면 셰이더에서 적용 안 함)
		FlipbookRender()->GetMaterial()->SetScalar(FLOAT_0, 0.f);
	}
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

void CPlayerScript::TakeDamage(int _Damage)
{
	// 무적 상태면 무시
	if (m_IsInvincible) 
		return;

	m_CurrentHP -= _Damage;

 	int HP = m_CurrentHP;

	ChangeState(PLAYER_STATE_ID::HIT);

	m_InvincibleTimer = 0.f;

	// 사망 체크
	if (m_CurrentHP <= 0)
	{
		// TODO: DIE 상태로 전환
		// ChangeState(PLAYER_STATE_ID::DIE);
	}
}


void CPlayerScript::Skill()
{
	if (KEY_TAP(KEY::SPACE))
	{
		Vec3 vMyPos = Transform()->GetRelativePos();
		Vec3 vMyScale = Transform()->GetRelativeScale();
		Vec3 vRotation = Transform()->GetRelativeRot();
		Vec3 vUp = Transform()->GetDir(DIR::UP);

		Instantiate(m_Missile.Get(), 4, vMyPos + vMyScale * 0.5f * vUp);

		// 미사일 생성
		//GameObject* pObject = nullptr;
		//pObject = new GameObject;
		//pObject->SetName(L"Missile");

		//pObject->AddComponent(new CTransform);
		//pObject->AddComponent(new CMeshRender);
		//pObject->AddComponent(new CCollider2D);

		//Ptr<CMissileScript> pMissileScript = new CMissileScript;
		//pMissileScript->SetTarget(m_Target);
		//pObject->AddComponent(pMissileScript.Get()); // 부모 포인터

		//Vec3 pPlayerPos = GetOwner()->Transform()->GetRelativePos();
		//Vec3 pPlayerScale = GetOwner()->Transform()->GetRelativeScale();
		//Vec3 pRight = GetOwner()->Transform()->GetDir(DIR::RIGHT);

		//pObject->Transform()->SetRelativePos(pPlayerPos + pPlayerScale * 0.5f * pRight);
		//pObject->Transform()->SetRelativeRot(GetOwner()->Transform()->GetRelativeRot());
		//pObject->Transform()->SetRelativeScale(Vec3(70.f, 70.f, 1.f));

		//pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
		//pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"Std2DMtrl"));

		//CreateObject(pObject, 4);

		// Player 와 Child 부모자식 연결
		//GetOwner()->AddChild(pObject);

	/*	TaskInfo info = {};
		info.Type = TASK_TYPE::DESTROY_OBJECT;
		info.Param_0 = (DWORD_PTR)pObject;
		TaskMgr::GetInst()->AddTask(info);*/

		//LevelMgr::GetInst()->GetCurrentLevel()->AddObject(0, pObject);
	} /// pObject 가 소멸되는 시점 /// 미사일 레퍼런스 카운트를 0으로 만들어서 미사일이 사라짐
	/// 전달되는 과정에서 오브젝트가 지워지게 되는 상황
	/// info.Param_0 에서 스마트 포인터로 받았으면 TaskMgr 가 pObject 를 가지고 있는 상황이 되니까 지워지지 않음
	/// 하지만 일반 포인터로 받으면 delete 도 안될거고 ~ 


	if (KEY_TAP(KEY::Z))
	{
		Destroy();
	}
}

void CPlayerScript::Slide()
{
}

void CPlayerScript::SetDefaultCollider()
{
	GetOwner()->Collider2D()->SetOffset(Vec2(-0.02f, -0.26f));
	GetOwner()->Collider2D()->SetScale(Vec2(0.23f, 0.34f));
}

void CPlayerScript::SetSlideCollider()
{
	GetOwner()->Collider2D()->SetOffset(Vec2(-0.02f, -0.34f));
	GetOwner()->Collider2D()->SetScale(Vec2(0.23f, 0.17f));
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

	if (!m_IsLand
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
