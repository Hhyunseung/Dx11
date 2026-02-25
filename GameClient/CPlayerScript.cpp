#include "pch.h"
#include "CPlayerScript.h"

#include "AssetMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "LevelMgr.h"
#include "RenderMgr.h"
#include "TaskMgr.h"

#include "GameObject.h"

CPlayerScript::CPlayerScript()
	: m_Land(true)
	, m_Jump(false)
	, m_DoubleJump(false)
{

}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Tick()
{
	Jump();

	Move();

	Shoot();


	//if (KEY_PRESSED(KEY::X))
	//{
	//	MeshRender()->GetMtrl()->SetScalar(INT_0, 1);
	//}
	//else
	//{
	//	MeshRender()->GetMtrl()->SetScalar(INT_0, 0);
	//}

	//Ptr<GameObject> pChild = GetOwner()->GetChild(0);

	//Vec3 vRelativePos = pChild->Transform()->GetRelativePos();
	//Vec3 vWorldPos = pChild->Transform()->GetWorldPos();
}
void CPlayerScript::Move()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vScale = GetOwner()->Transform()->GetRelativeScale();
	Vec3 vRotation = GetOwner()->Transform()->GetRelativeRot();

	Vec3 vUp = GetOwner()->Transform()->GetDir(DIR::UP);
	Vec3 vDown = -vUp;



	if (KEY_PRESSED(KEY::UP))
		vPos += vUp * 150.f * DT;
	if (KEY_PRESSED(KEY::DOWN))
		vPos += vDown * 150.f * DT;

	if (KEY_PRESSED(KEY::RIGHT))
		vRotation.z -= XM_PI * DT;
	if (KEY_PRESSED(KEY::LEFT))
		vRotation.z += XM_PI * DT;


	GetOwner()->Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativeScale(vScale);
	GetOwner()->Transform()->SetRelativeRot(vRotation);

}

void CPlayerScript::Shoot()
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
	if (KEY_TAP(KEY::SPACE))
	{
		if (!m_Land)
		{
			m_DoubleJump = true;

			GetOwner()->FlipbookRender()->Play(2, 8.f, 1);
		}

		else
		{
			m_Land = false;
			m_Jump = true;

			GetOwner()->FlipbookRender()->Play(1, 8.f, 1);
		}
	}


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

