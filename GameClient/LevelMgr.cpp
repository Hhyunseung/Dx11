#include "pch.h"
#include "LevelMgr.h"

#include "components.h"

#include "CCamMoveScript.h"
#include "CPlayerScript.h"
#include "CCamera.h"
#include "CMonsterScript.h"

#include "Device.h"
#include "CollisionMgr.h"
#include "AssetMgr.h"

LevelMgr::LevelMgr()
{

}

LevelMgr::~LevelMgr()
{

}

void LevelMgr::Init()
{
	// Level 생성
	m_CurLevel = new ALevel;
	m_CurLevel->SetName(L"Current Level");

	m_CurLevel->GetLayer(0)->SetName(L"Default");
	m_CurLevel->GetLayer(1)->SetName(L"Background");
	m_CurLevel->GetLayer(2)->SetName(L"Tile");
	m_CurLevel->GetLayer(3)->SetName(L"Player");
	m_CurLevel->GetLayer(4)->SetName(L"PlayerProjectile");

	m_CurLevel->GetLayer(5)->SetName(L"Enermy");
	m_CurLevel->GetLayer(6)->SetName(L"EnermyProjectile");

	Ptr<GameObject> pObject = nullptr;

	// 카메라 역할 오브젝트
	pObject = new GameObject;
	pObject->SetName(L"MainCamera");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCamera);
	pObject->AddComponent(new CCamMoveScript);

	pObject->Camera()->LayerCheckAll();
	//pObject->Camera()->LayerCheck(31); // 31 -> UI 레이어

	pObject->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pObject->Camera()->SetFar(10000.f);
	pObject->Camera()->SetFOV(90.f);
	pObject->Camera()->SetOrthoScale(1.f);
	Vec2 vResolution = Device::GetInst()->GetRenderResolution();
	pObject->Camera()->SetAspectRatio(vResolution.x / vResolution.y); // 종횡비(AspectRatio)
	pObject->Camera()->SetWidth(vResolution.x); // 직교 투영 가로 길이

	m_CurLevel->AddObject(0, pObject);

	
	// 광원 추가
	pObject = new GameObject;
	pObject->SetName(L"Light_1");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObject->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
	pObject->Light2D()->SetRadius(300.f);
	pObject->Transform()->SetRelativePos(Vec3(-150.f, 0.f, 0.f));

	m_CurLevel->AddObject(0, pObject);


	//// 광원 추가
	//pObject = new GameObject;
	//pObject->SetName(L"Light_2");
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CLight2D);

	//pObject->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	//pObject->Light2D()->SetLightColor(Vec3(0.3f, 0.3f, 1.f));
	////pObject->Light2D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
	//pObject->Light2D()->SetRadius(200.f);
	//pObject->Light2D()->SetAngle(XM_PI / 4.f);

	//pObject->Transform()->SetRelativePos(Vec3(-250.f, 0.f, 0.f));
	//pObject->Transform()->SetRelativeRot(Vec3(0.f, 0.f, XM_PI / 4.f));

	//m_CurLevel->AddObject(0, pObject);



	// 적 오브젝트 생성
	for (int i = 0; i < 5; ++i)
	{
		Ptr<GameObject> pMonster = new GameObject;
		pMonster->SetName(L"Monster");

		pMonster->AddComponent(new CTransform);
		pMonster->AddComponent(new CMeshRender);
		pMonster->AddComponent(new CCollider2D);
		pMonster->AddComponent(new CMonsterScript);

		pMonster->Transform()->SetRelativePos(Vec3(300.f * (float)i, 0.f, 100.f));
		pMonster->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

		//pMonster->SpriteRender()->SetSprite(FIND(ASprite, L"TileSprite_47"));
		pMonster->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
		pMonster->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"MonsterMtrl"));

		m_CurLevel->AddObject(5, pMonster);
	}

	// 플레이어 오브젝트 생성
	pObject = new GameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CFlipbookRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CPlayerScript);
	
	//Ptr<CPlayerScript> pPlayerScript = new CPlayerScript;
	//pPlayerScript->SetTarget(pMonster);
	//pObject->AddComponent(pPlayerScript.Get());


	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObject->Collider2D()->SetScale(Vec2(0.25f, 0.25f));
	pObject->Collider2D()->SetOffset(Vec2(0.5f, 0.f));

	pObject->FlipbookRender()->AddFlipbook(FIND(AFlipbook, L"DragonCookie_Idle"));
	pObject->FlipbookRender()->AddFlipbook(FIND(AFlipbook, L"DragonCookie_Jump"));
	pObject->FlipbookRender()->AddFlipbook(FIND(AFlipbook, L"DragonCookie_DoubleJump"));
	pObject->FlipbookRender()->Play(0, 8.f, -1);


	// 자식 오브젝트 생성
	Ptr<GameObject> pChild = new GameObject;
	pChild->SetName(L"Child");

	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);
	pChild->AddComponent(new CCollider2D);

	/// 부모의 상대적인 위치와 크기로 설정해야함
	pChild->Transform()->SetRelativePos(Vec3(-50.f, 0.f, 0.f));
	pChild->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	pChild->Transform()->SetIndependentScale(true);

	pChild->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
	pChild->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"Std2DMtrl"));

	// Player 와 Child 부모자식 연결
	pObject->AddChild(pChild);

	// Player(부모 오브젝트) 를 레벨에 추가
	m_CurLevel->AddObject(3, pObject);


	// Back Ground - 가로로 반복
	for (int i = 0; i < 3; ++i)  // 3개의 배경을 가로로 배치
	{
		pObject = new GameObject;
		wchar_t nameBuff[50] = {};
		swprintf_s(nameBuff, L"BackGround_1_%d", i);
		pObject->SetName(nameBuff);

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetRelativePos(Vec3(-50.f + (2066.f * i), 0.f, 900.f));  // 가로로 배치
		pObject->Transform()->SetRelativeScale(Vec3(2066.f, 740.f, 1.f));

		pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"BackGround_Ship_Stage1_1_Mtrl"));

		m_CurLevel->AddObject(1, pObject);
	}

	// Back Ground - 가로로 반복
	for (int i = 0; i < 3; ++i)  // 5개의 배경을 가로로 배치
	{
		pObject = new GameObject;
		wchar_t nameBuff[50] = {};
		swprintf_s(nameBuff, L"BackGround_2_%d", i);
		pObject->SetName(nameBuff);

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetRelativePos(Vec3(-50.f + (2066.f * i), 0.f, 800.f));  // 가로로 배치
		pObject->Transform()->SetRelativeScale(Vec3(2066.f, 740.f, 1.f));

		pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"BackGround_Ship_Stage1_2_Mtrl"));

		m_CurLevel->AddObject(1, pObject);
	}


	for (int i = 0; i < 10; ++i)
	{
		Ptr<GameObject> pObject = new GameObject;
		wchar_t nameBuff[50] = {};
		swprintf_s(nameBuff, L"Tile_1_%d", i);
		pObject->SetName(nameBuff);

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CCollider2D);

		pObject->Transform()->SetRelativePos(Vec3(-100.f + (124.f * i), -200.f, 700.f));
		pObject->Transform()->SetRelativeScale(Vec3(124.f, 140.f, 1.f));

		pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"Tile_Ship_Stage1_1_Mtrl"));

		m_CurLevel->AddObject(2, pObject);
	}


	// Tile Object
	Ptr<GameObject> pTileObj = new GameObject;

	pTileObj->AddComponent(new CTransform);
	pTileObj->AddComponent(new CCollider2D);
	pTileObj->AddComponent(new CTileRender);

	pTileObj->Transform()->SetRelativePos(Vec3(-640.f, 640.f, 500.f));
	// pTileObj->Transform()->GetRelativeScale  // 내가 어떤 타일맵을 골랐는지에 따라.. CTileRender에서 크키조절
	pTileObj->TileRender()->SetTileMap(FIND(ATileMap, L"TestTileMap"));
	
	//m_CurLevel->AddObject(2, pTileObj);


	// 레벨 충돌 설정
	m_CurLevel->CheckCollisionLayer(3, 5);
	m_CurLevel->CheckCollisionLayer(4, 5);
	m_CurLevel->CheckCollisionLayer(3, 6);


	// 레벨 시작
	m_CurLevel->SetChanged(); // 레벨 상태 변경
	m_CurLevel->Begin();
}

void LevelMgr::Progress()
{
	// 이전에 등록된 모든 오브젝트들 제거
	m_CurLevel->Deregister();

	// 레벨 안에 잇는 오브젝트들이 이번 DT 동안 할 일 수행
	m_CurLevel->Tick();
	/// 모든 오브젝트들의 Tick 이 끝난후 뒷작업 ex 월드 행렬 계산
	m_CurLevel->FinalTick();

	// 충돌 검사 진행
	CollisionMgr::GetInst()->Progress(m_CurLevel);
}

Ptr<GameObject> LevelMgr::FindObjectByName(const wstring& _name)
{
	return m_CurLevel->FindObjectByName(_name);
}
