#include "pch.h"
#include "CPlayerScript.h"

#include "AssetMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "LevelMgr.h"
#include "RenderMgr.h"
#include "TaskMgr.h"
#include "CStateMachine.h"

#include "GameObject.h"

#include "CRunState.h"
#include "CJumpState.h"

CPlayerScript::CPlayerScript()
	: CScript(SCRIPT_TYPE::PLAYERSCRIPT)
	, m_Land(true)
	, m_BodyCollider(nullptr)
	, m_FeetCollider(nullptr)
	, m_Jump(false)
	, m_DoubleJump(false)
{

}

CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::Begin()
{
	m_StateMachine = new CStateMachine;

	// 상태 등록
	m_StateMachine->AddState(new CRunState(this));
	m_StateMachine->AddState(new CJumpState(this));

	m_StateMachine->StartState(PLAYER_STATE_ID::RUN);


	Ptr<GameObject> pChild = nullptr;
	
	pChild = GetOwner()->GetChild(1);
	pChild->SetLayerIdx(4); // PlayerFeet 레이어
	m_FeetCollider = pChild->Collider2D().Get();

	
	m_FeetCollider->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CPlayerScript::FeetBeginOverlap);
	m_FeetCollider->AddDynamicOverlap(this, (COLLISION_EVENT)&CPlayerScript::FeetOverlap);
	m_FeetCollider->AddDynamicEndOverlap(this, (COLLISION_EVENT)&CPlayerScript::FeetEndOverlap);

	pChild = GetOwner()->GetChild(2);
	m_BodyCollider = pChild->Collider2D().Get();

	m_BodyCollider->AddDynamicBeginOverlap(this, (COLLISION_EVENT)&CPlayerScript::BeginOverlap);
	
}

void CPlayerScript::Tick()
{
	m_PrevFeetY = m_FeetCollider->GetBottomY();

	if (KEY_TAP(KEY::SPACE))
	{
		if (!m_Land)
		{
			m_DoubleJump = true;
			m_StateMachine->ChangeState(PLAYER_STATE_ID::JUMP);
		}

		else
		{
			m_Land = false;
			m_Jump = true;
			GetOwner()->FlipbookRender()->Play(1, 8.f, 1);
		}
	}

	m_StateMachine->Tick();








	//Jump();

	//Move();

	//Skill();


	//if (KEY_PRESSED(KEY::X))
	//{
	//	MeshRender()->GetMtrl()->SetScalar(INT_0, 1);
	//}
	//else
	//{
	//	MeshRender()->GetMtrl()->SetScalar(INT_0, 0);
	//}

	m_CurFeetY = m_FeetCollider->GetBottomY();
}

void CPlayerScript::Move()
{

}

void CPlayerScript::Skill()
{
	if (KEY_TAP(KEY::SPACE))
	{
		GameObject* pObject = nullptr;

		// 미사일 생성
		pObject = new GameObject;
		pObject->SetName(L"Missile");

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CCollider2D);

		Ptr<CMissileScript> pMissileScript = new CMissileScript;
		pMissileScript->SetTarget(m_Target);
		pObject->AddComponent(pMissileScript.Get()); // 부모 포인터

		Vec3 pPlayerPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 pPlayerScale = GetOwner()->Transform()->GetRelativeScale();
		Vec3 pRight = GetOwner()->Transform()->GetDir(DIR::RIGHT);

		pObject->Transform()->SetRelativePos(pPlayerPos + pPlayerScale * 0.5f * pRight);
		pObject->Transform()->SetRelativeRot(GetOwner()->Transform()->GetRelativeRot());
		pObject->Transform()->SetRelativeScale(Vec3(70.f, 70.f, 1.f));

		pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"Std2DMtrl"));

		CreateObject(pObject, 4);

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

void CPlayerScript::Jump()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	// 점프해서 위로 올라가는 느낌
	float jumpPower = 500.f;
	static float jumpTime = 0.2f;
	

	static float jumpDuration = 0.f;

	if (m_Jump)
	{
		jumpDuration += DT;
		vPos.y += jumpPower * DT;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}

	float gravety = 1000.f;
	static float dropSpeed = 0.f;

	// 점프 시간 종료하면 다시 내려오는 느낌
	if (jumpDuration >= jumpTime)
	{
		m_Jump = false;

		dropSpeed += gravety * DT;
		vPos.y -= dropSpeed * DT;

		GetOwner()->Transform()->SetRelativePos(vPos);
	}

	// 더블 점프
	if (m_DoubleJump)
	{
		jumpDuration = 0.f;
		dropSpeed = 0.f;
		m_Jump = true;
		m_DoubleJump = false;
	}
}

void CPlayerScript::Slide()
{
}


void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::FeetBeginOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::FeetOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{
	if ((m_FeetCollider->GetBottomY() <= _OtherCollider->GetTopY())
		&& (m_CurFeetY >= m_FeetCollider->GetBottomY()))
	{
		float playerBottomY = m_FeetCollider->GetBottomY();
		float PlatformTopY = _OtherCollider->GetTopY();
		int a = 0;
	}
}

void CPlayerScript::FeetEndOverlap(CCollider2D* _OwnCollider, CCollider2D* _OtherCollider)
{

}