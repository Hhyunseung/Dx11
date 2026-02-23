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
{

}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Tick()
{
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

	Ptr<GameObject> pChild = GetOwner()->GetChild(0);

	Vec3 vRelativePos = pChild->Transform()->GetRelativePos();
	Vec3 vWorldPos = pChild->Transform()->GetWorldPos();

	//Ptr<CCamera> pCam = RenderMgr::GetInst()->GetPOVCamera();
//Vec3 vCamPos = pCam->Transform()->GetPos();
//Vec3 vMyPos = Transform()->GetPos();
//Vec3 vDirToCam = vCamPos - vMyPos;
//vDirToCam.Normalize();

//Vec3 vBot = Vec3(vDirToCam.x, 0.f, vDirToCam.z).Normalize();
//Vec3 vFront = Vec3(0.f, 0.f, -1.f);
//	
//float YDot = vBot.Dot(vFront);
//float YAngle = acosf(YDot);

//if (vFront.Cross(vBot).y < 0.f)
//	YAngle = XM_2PI - YAngle;
//	
//float XDot = vBot.Dot(vDirToCam);
//if (XDot > 1.f) XDot = 1.f;
//if (XDot < -1.f) XDot = -1.f;

//float XAngle = acosf(XDot);
//if(vCamPos.y < vMyPos.y)
//	XAngle = XM_2PI - XAngle;

//Transform()->SetRotation(Vec3(0.f, YAngle, 0.f));
//Transform()->SetRotation(Vec3(XAngle, 0.f, 0.f));
//Transform()->SetRotation(Vec3(XAngle, YAngle, 0.f));
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

	if (KEY_TAP(KEY::SPACE))
	{
		//DrawDebugRect(Transform()->GetRelativePos()
		//	, Transform()->GetRelativeScale() * Vec3(2.f, 2.f, 2.f),
		//	Vec3(0.f, 0.f, 0.f), Vec4(1.f, 1.f, 0.f, 1.f), 2.f);

		//DrawDebugRect(Transform()->GetWorldMat(), Vec4(1.f, 1.f, 0.f, 1.f), 2.f);

		//DrawDebugCircle(Transform()->GetRelativePos(), 100.f, Vec4(1.f, 0.f, 0.f, 1.f), 2.f);
	}

	if (KEY_TAP(KEY::Z))
	{
		Destroy();
	}
}

