#include "pch.h"
#include "AssetMgr.h"

#include "PathMgr.h"

void AssetMgr::Init()
{
	CreateEngineMesh();
	CreateEngineShader();
	CreateEngineTexture();
	CreateEngineMaterial();
	CreateEngineSprite();
}

void AssetMgr::CreateEngineMesh()
{
	Ptr<AMesh> pMesh = nullptr;


	// 정점 기본 값 세팅
	Vtx arrVtx[4] = {};

	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);
	arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 0.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 0.f, 0.f);

	// ============ 사각형 메쉬 생성 =================
	UINT arrIdx[6] = {
		0, 2, 3,
		0, 1, 2
	};

	pMesh = new AMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 6);

	// 에셋매니저에 등록
	AddAsset(L"RectMesh", pMesh.Get());

	// =====================
	// RectMesh_LineStrip
	// =====================
	arrIdx[0] = 0;
	arrIdx[1] = 1;
	arrIdx[2] = 2;
	arrIdx[3] = 3;
	arrIdx[4] = 0;

	pMesh = new AMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 5);

	// 에셋매니저에 등록
	AddAsset(L"RectMesh_LineStrip", pMesh.Get());



	// ============ 삼각형 메쉬 생성 =============
	Vtx arr[3] = {};
	arr[0].vPos = Vec3(0.f, 1.f, 0.f);
	arr[0].vColor = Vec4(0.f, 1.f, 0.f, 0.f);

	arr[1].vPos = Vec3(1.f, -1.f, 0.f);
	arr[1].vColor = Vec4(1.f, 0.f, 0.f, 0.f);

	arr[2].vPos = Vec3(-1.f, -1.f, 0.f);
	arr[2].vColor = Vec4(0.f, 0.f, 1.f, 0.f);

	UINT idx[3] = { 0, 1, 2 };

	pMesh = new AMesh;
	pMesh->Create(arr, 3, idx, 3);

	// 에셋매니저에 등록
	AddAsset(L"TriMesh", pMesh.Get());


	// ================ 원 메쉬 생성 ==============//
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;

	// 중점
	Vtx v;
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	float Theta = 0.f;
	float Radius = 0.5f;
	float Slice = 40.f; /// 몇각형으로 만들지

	// 원의 테두리 정점 추가
	for (int i = 0; i < (int)Slice + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf(Theta), Radius * sinf(Theta), 0.f); // R * cosθ , R * sinθ
		//v.vUV = Vec2(0.5f, 0.5f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		Theta += XM_2PI / Slice; // 360 / Slice
	}

	// 인덱스
	for (int i = 0; i < (int)Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new AMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(L"CircleMesh", pMesh.Get());
	
	// ===================
	// CircleMesh_LineStrip
	// ===================

	vecIdx.clear();
	for (int i = 0; i < (int)Slice + 1; ++i)
	{
		vecIdx.push_back(i + 1);
	}

	pMesh = new AMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddAsset(L"CircleMesh_LineStrip", pMesh.Get());
}

void AssetMgr::CreateEngineShader()
{
	// 파이프라인(쉐이더) 생성
	Ptr<AGraphicShader> pShader = nullptr;


	//================== Std2D 생성 ==============//
	pShader = new AGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	AddAsset(L"Std2DShader", pShader.Get());


	//================== DbgShader 생성 ==============//
	pShader = new AGraphicShader;
	pShader->CreateVertexShader(L"Shader\\dbg.fx", "VS_Debug");
	pShader->CreatePixelShader(L"Shader\\dbg.fx", "PS_Debug");
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	AddAsset(L"DbgShader", pShader.Get());
}

void AssetMgr::CreateEngineTexture()
{
	// 이미지 한장 로딩해보기
	Ptr<ATexture> pTex = nullptr;
	pTex = new ATexture;
	wstring FilePath = CONTENT_PATH;
	FilePath += L"Texture\\Character.png";
	pTex->Load(FilePath);
	AddAsset(L"PlayerImage", pTex.Get());

	pTex = new ATexture;
	FilePath = CONTENT_PATH;
	FilePath += L"Texture\\Missile.png";
	pTex->Load(FilePath);
	AddAsset(L"Missile", pTex.Get());

	pTex = new ATexture;
	FilePath = CONTENT_PATH;
	FilePath += L"Texture\\Enemy.png";
	pTex->Load(FilePath);
	AddAsset(L"Enemy", pTex.Get());

	pTex = new ATexture;
	FilePath = CONTENT_PATH;
	FilePath += L"Texture\\Star.png";
	pTex->Load(FilePath);
	AddAsset(L"Star", pTex.Get());

	pTex = new ATexture;
	FilePath = CONTENT_PATH;
	FilePath += L"Texture\\Sol.png";
	pTex->Load(FilePath);
	AddAsset(L"Sol", pTex.Get());

	pTex = new ATexture;
	FilePath = CONTENT_PATH;
	FilePath += L"Texture\\BackGround.png";
	pTex->Load(FilePath);
	AddAsset(L"BackGround", pTex.Get());

	pTex = new ATexture;
	FilePath = CONTENT_PATH;
	FilePath += L"Texture\\link.png";
	pTex->Load(FilePath);
	AddAsset(L"Link", pTex.Get());
}

void AssetMgr::CreateEngineMaterial()
{
	// 재질 만들기
	Ptr<AMaterial> pMtrl = nullptr;

	//================== Std2DMtrl 생성 ==============//
	pMtrl = new AMaterial;
	pMtrl->SetName(L"Std2DMtrl");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"Std2DShader"));
	pMtrl->SetTexture(TEX_0, FindAsset<ATexture>(L"PlayerImage"));
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	//================== DbgMtrl 생성 ==============//
	pMtrl = new AMaterial;
	pMtrl->SetName(L"DbgMtrl");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"DbgShader"));


	AddAsset(pMtrl->GetName(), pMtrl.Get());
}

void AssetMgr::CreateEngineSprite()
{
	Ptr<ATexture> pAtlas = FIND(ATexture, L"Link");
	float Width = pAtlas->GetWidth();
	float Height = pAtlas->GetHeight();
	Vec2 SlicePixel = Vec2(120.f, 130.f);
	
	Ptr<ASprite> pSprite = nullptr;
	for (int i = 0; i < 10; i++)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMD_%d", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 4.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}

	for (int i = 0; i < 10; i++)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkML_%d", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 5.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}

	for (int i = 0; i < 10; i++)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMU_%d", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 6.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}

	for (int i = 0; i < 10; i++)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMR_%d", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 7.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}
	

	Ptr<AFlipbook> pFlipbook = nullptr;
	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Link_MoveDown");

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMD_%d", i);
		pFlipbook->AddSprite(FIND(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Link_MoveLeft");
	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkML_%d", i);
		pFlipbook->AddSprite(FIND(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Link_MoveUp");

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMU_%d", i);
		pFlipbook->AddSprite(FIND(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Link_MoveRight");

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"LinkMR_%d", i);
		pFlipbook->AddSprite(FIND(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());
}
