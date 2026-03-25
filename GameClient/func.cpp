#include "pch.h"

#include "RenderMgr.h"
#include "TaskMgr.h"

void CreateObject(GameObject* _Object, int LayerIdx)
{
	TaskInfo info = {};

	info.Type = TASK_TYPE::CREATE_OBJECT;
	info.Param_0 = (DWORD_PTR)_Object;
	info.Param_1 = LayerIdx;

	TaskMgr::GetInst()->AddTask(info);
}

wchar_t Buff[255] = {};
void ChangeLevel(const wstring& _NextLevelName)
{
	TaskInfo info = {};

	wcscpy_s(Buff, 255, _NextLevelName.c_str());

	info.Type = TASK_TYPE::CHANGE_LEVEL;
	info.Param_0 = (DWORD_PTR)Buff;

	TaskMgr::GetInst()->AddTask(info);
}

void ChangeLevelState(LEVEL_STATE _NextState)
{
	TaskInfo info = {};

	info.Type = TASK_TYPE::CHANGE_LEVEL_STATE;
	info.Param_0 = (DWORD_PTR)_NextState;

	TaskMgr::GetInst()->AddTask(info);
}

void DrawDebugRect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rot, Vec4 _Color, float _Duration, bool _DeapthTest)
{
	DbgInfo info = {};
	info.Shape = DBG_SHAPE::RECT;
	info.Pos = _Pos;
	info.Scale = _Scale;
	info.Rotation = _Rot;

	info.matWorld = XMMatrixIdentity();

	info.Color = _Color;
	info.Age = 0.f;
	info.Life = _Duration; // 0.f 로 하면 딱 한번 렌더링 

	info.DepthTest = _DeapthTest;

	RenderMgr::GetInst()->AddDebugInfo(info);
}

void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, float _Duration, bool _DeapthTest)
{
	DbgInfo info = {};
	info.Shape = DBG_SHAPE::RECT;
	
	info.matWorld = _matWorld;

	info.Color = _Color;
	info.Age = 0.f;
	info.Life = _Duration; // 0.f 로 하면 딱 한번 렌더링 

	info.DepthTest = _DeapthTest;

	RenderMgr::GetInst()->AddDebugInfo(info);
}

void DrawDebugCircle(Vec3 _Pos, float _Radius, Vec4 _Color, float _Duration, bool _DeapthTest)
{
	DbgInfo info = {};
	info.Shape = DBG_SHAPE::CIRCLE;
	info.Pos = _Pos;
	info.Scale = Vec3(_Radius * 2.f, _Radius * 2.f, 0.f); // 반지름 0.5f 로 설정했었으니까 
	info.Rotation = Vec3(0.f, 0.f, 0.f);
	info.Color = _Color;
	info.Age = 0.f;
	info.Life = _Duration; // 0.f 로 하면 딱 한번 렌더링 
	info.DepthTest = _DeapthTest;

	RenderMgr::GetInst()->AddDebugInfo(info);
}

void SaveWString(FILE* _File, const wstring& _String)
{
	int Len = _String.length();
	fwrite(&Len, sizeof(int), 1, _File);
	fwrite(_String.data(), sizeof(wchar_t), Len, _File);
}

wstring LoadWString(FILE* _File)
{
	int Len = 0;
	fread(&Len, sizeof(int), 1, _File);

	wchar_t buff[255] = {};
	fread(buff, sizeof(wchar_t), Len, _File);

	return buff;
}

void SaveAssetRef(FILE* _File, Asset* _Asset)
{
	// Asset 이 Null 인지 아닌지 저장
	bool IsNull = _Asset;
	fwrite(&IsNull, sizeof(bool), 1, _File);

	// Asset 의 Key, RelativePath 저장
	if (nullptr != _Asset)
	{
		SaveWString(_File, _Asset->GetKey());
		SaveWString(_File, _Asset->GetRelativePath());
	}
}


float Saturate(float _Data)
{
	if (1.f < _Data)
		return 1.f;
	else if (_Data < 0.f)
		return 0.f;
	else
		return _Data;
}

bool IsValid(Ptr<GameObject>& _Object)
{
	if (nullptr == _Object || _Object->IsDead())
	{
		_Object = nullptr; // 원본을 nullptr 로 수정
		return false;
	}

	return true;
}

#include "ALevel.h"
#include "GameObject.h"

#include "AssetMgr.h"
#include "Device.h"
#include "CollisionMgr.h"
#include "GamePlayMgr.h"

#include "Source/Scripts/CCamMoveScript.h"
#include "Source/Scripts/CPlayerScript.h"
#include "Source/Scripts/CMonsterScript.h"
#include "Source/Scripts/CStateMachine.h"
#include "Source/Scripts/CScrollScript.h"
#include "Source/Scripts/CBGScrollScript.h"

void CreateTestLevel()
{
	{
		Ptr<ALevel> pLevel = LOAD(ALevel, L"Level\\TestLevel.lv");
		Ptr<APrefab> pPrefab = FIND(APrefab, L"Prefab\\TimeKeeperCookie.pref");

		Ptr<AStageData> pStageData = LOAD(AStageData, L"StageData\\TestStage.stage");

		GamePlayMgr::GetInst()->SetSelectedCharacterPrefab(pPrefab);
		GamePlayMgr::GetInst()->SetStageData(pStageData);
		
		Ptr<GameObject> pObject = new GameObject;
		pObject->SetName(L"FlipbookDefault");

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CFlipbookRender);
		pObject->AddComponent(new CCollider2D);

		pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
		pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

		pObject->Collider2D()->SetScale(Vec2(1.f, 1.f));
		pObject->Collider2D()->SetOffset(Vec2(0.f, 0.f));

		//// Save 후 Load
		pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\Jelly_BearRainbow_z65.flip"));
		pObject->FlipbookRender()->Play(0, 12.f, -1);

		pLevel->AddObject(0, pObject);
		
		
		ChangeLevel(L"Level\\TestLevel.lv");
	}

	return;


	// Level 생성
	Ptr<ALevel> pLevel = new ALevel;
	pLevel->SetName(L"Current Level");

	pLevel->GetLayer(0)->SetName(L"Default");
	pLevel->GetLayer(1)->SetName(L"Background");
	pLevel->GetLayer(2)->SetName(L"Tile");
	pLevel->GetLayer(3)->SetName(L"Player");
	pLevel->GetLayer(4)->SetName(L"PlayerFeet");
	pLevel->GetLayer(5)->SetName(L"PlayerProjectile");

	pLevel->GetLayer(6)->SetName(L"Enermy");
	pLevel->GetLayer(7)->SetName(L"EnermyProjectile");


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

	pLevel->AddObject(0, pObject);


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

	pLevel->AddObject(0, pObject);


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
		pMonster->AddComponent(new CScrollScript);
		pMonster->AddComponent(new CMonsterScript);

		pMonster->Transform()->SetRelativePos(Vec3(300.f * (float)i, 0.f, 100.f));
		pMonster->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

		//pMonster->SpriteRender()->SetSprite(FIND(ASprite, L"TileSprite_47"));
		pMonster->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
		pMonster->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"MonsterMtrl"));

		pLevel->AddObject(6, pMonster);
	}

	// 플레이어 오브젝트 생성
	//
	//pObject = new GameObject;
	//pObject->SetName(L"Player");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CFlipbookRender);
	//pObject->AddComponent(new CCollider2D);
	//pObject->AddComponent(new CPlayerScript);

	//pObject->Transform()->SetRelativePos(Vec3(0.f, 17.f, 100.f));
	//pObject->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	//pObject->Collider2D()->SetScale(Vec2(0.25f, 0.25f));
	//pObject->Collider2D()->SetOffset(Vec2(0.5f, 0.f));

	////// Save 후 Load
	//pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\TimeKeeperCookie_Idle.flip"));
	//pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\DragonCookie_Jump.flip"));
	//pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\DragonCookie_DoubleJump.flip"));
	//pObject->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\DragonCookie_Land.flip"));
	//
	//pObject->FlipbookRender()->Play(0, 8.f, -1);


	//// 자식 오브젝트 생성
	////////// 펫은 자식 오브젝트로 설정안하고 따로 따라오게 해야할듯..
	////////// 플레이어 뒤를 딜레이 주고 따라오는 느낌으로 만들어야
	//Ptr<GameObject> pChild = new GameObject;
	//pChild->SetName(L"CookiePet");

	//pChild->AddComponent(new CTransform);
	//pChild->AddComponent(new CFlipbookRender);

	///// 부모의 상대적인 위치와 크기로 설정해야함
	//pChild->Transform()->SetRelativePos(Vec3(-80.f, -65.f, 0.f));
	//pChild->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 1.f));
	//pChild->Transform()->SetIndependentScale(true);

	//pChild->FlipbookRender()->AddFlipbook(LOAD(AFlipbook, L"Flipbook\\DragonCookie_PetIdle.flip"));
	//pChild->FlipbookRender()->Play(0, 8.f, -1);


	//// Player 와 Child 부모자식 연결
	//pObject->AddChild(pChild);

	//// 발 센서 역할을 하는 자식 오브젝트
	//pChild = new GameObject;
	//pChild->SetName(L"FootSensor");

	//pChild->AddComponent(new CTransform);
	//pChild->AddComponent(new CCollider2D);

	//pChild->Transform()->SetRelativePos(Vec3(-5.f, -147.f, 0.f));
	//pChild->Transform()->SetRelativeScale(Vec3(60.f, 7.f, 1.f));
	//pChild->Transform()->SetIndependentScale(true);
	//
	////Ptr<CPlayerScript> pPlayerScript = new CPlayerScript;
	////pPlayerScript->SetFeetCollider(pChild);
	////pObject->AddComponent(pPlayerScript.Get());


	//// Player 와 Child 부모자식 연결
	//pObject->AddChild(pChild);


	//// 발 센서 역할을 하는 자식 오브젝트
	//pChild = new GameObject;
	//pChild->SetName(L"BodySensor");

	//pChild->AddComponent(new CTransform);
	//pChild->AddComponent(new CCollider2D);

	//pChild->Transform()->SetRelativePos(Vec3(-5.f, -126.f, 0.f));
	//pChild->Transform()->SetRelativeScale(Vec3(60.f, 60.f, 1.f));
	//pChild->Transform()->SetIndependentScale(true);
	//

	//// Player 와 Child 부모자식 연결
	//pObject->AddChild(pChild);


	//// Player(부모 오브젝트) 를 레벨에 추가
	//pLevel->AddObject(3, pObject);


	// 플레이어 오브젝트를 레벨에 추가한 후, 프리팹으로 저장
	//Ptr<APrefab> pCharacter = new APrefab;
	//pCharacter->SetObject(pObject);
	//AssetMgr::GetInst()->AddAsset(L"Prefab\\TimeKeeperCookie.pref", pCharacter.Get());
	//
	//wstring FilePath = CONTENT_PATH + L"Prefab\\TimeKeeperCookie.pref";
	//pCharacter->Save(FilePath);

	// 플레이어 오브젝트를 프리팹 불러오기
	//Ptr<APrefab> pCharacter = LOAD(APrefab, L"Prefab\\TimeKeeperCookie.pref");
	//pObject = pCharacter->Instantiate();

	pObject = new GameObject;
	pObject->SetName(L"BackGround_1_1");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CBGScrollScript);

	pObject->Transform()->SetRelativePos(Vec3(-50.f, 0.f, 900.f));  // 첫 배경 위치
	pObject->Transform()->SetRelativeScale(Vec3(2066.f, 740.f, 1.f));

	pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"BackGround_Ship_Stage1_1_Mtrl"));

	pLevel->AddObject(1, pObject);

	pObject = new GameObject;
	pObject->SetName(L"BackGround_2_1");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CBGScrollScript);

	pObject->Transform()->SetRelativePos(Vec3(-50.f, 0.f, 800.f));  // 첫 배경 위치
	pObject->Transform()->SetRelativeScale(Vec3(2066.f, 740.f, 1.f));

	pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"BackGround_Ship_Stage1_2_Mtrl"));

	pLevel->AddObject(1, pObject);


	for (int i = 0; i < 10; ++i)
	{
		Ptr<GameObject> pObject = new GameObject;
		wchar_t nameBuff[50] = {};
		swprintf_s(nameBuff, L"Tile_1_%d", i);
		pObject->SetName(nameBuff);

		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->AddComponent(new CCollider2D);
		pObject->AddComponent(new CScrollScript);

		pObject->Transform()->SetRelativePos(Vec3(-740.f + (124.f * i), -300.f, 700.f));
		pObject->Transform()->SetRelativeScale(Vec3(124.f, 140.f, 1.f));

		pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"Tile_Ship_Stage1_1_Mtrl"));

		pLevel->AddObject(2, pObject);
	}


	// Tile Object
	Ptr<GameObject> pTileObj = new GameObject;

	pTileObj->AddComponent(new CTransform);
	pTileObj->AddComponent(new CCollider2D);
	pTileObj->AddComponent(new CTileRender);

	pTileObj->Transform()->SetRelativePos(Vec3(-640.f, 640.f, 500.f));
	// pTileObj->Transform()->GetRelativeScale  // 내가 어떤 타일맵을 골랐는지에 따라.. CTileRender에서 크키조절
	pTileObj->TileRender()->SetTileMap(LOAD(ATileMap, L"TileMap//TestTileMap"));

	//m_CurLevel->AddObject(2, pTileObj);


	// 레벨 충돌 설정
	pLevel->CheckCollisionLayer(2, 4);
	//pLevel->CheckCollisionLayer(3, 5);
	pLevel->CheckCollisionLayer(3, 6);


	// 레벨 시작
	// 레벨 변경점 체크
	pLevel->SetChanged(); // 레벨 상태 변경

	// 레벨을 AssetMgr 에 등록
	AssetMgr::GetInst()->AddAsset(L"TestLevel", pLevel.Get());

	// 레벨을 파일로 저장
	wstring ContentPath = CONTENT_PATH;
	pLevel->Save(ContentPath + L"Level\\TestLevel.lv");

	// 레벨을 변경 /// 즉시 바꾸면 안됨. 호출되는 시점이 Tick 중간일수도 있으니까... TaskMgr 을 이용해서 다음 프레임에
	ChangeLevel(L"TestLevel");
}