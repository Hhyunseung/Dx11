#include "pch.h"
#include "AssetMgr.h"

#include <filesystem>
#include "PathMgr.h"

void AssetMgr::Init()
{
	CreateEngineMesh();
	CreateEngineShader();
	CreateEngineTexture();
	CreateEngineMaterial();
	CreateEngineSprite();
	CreateEnginePrefab();

	LoadAllSprite();
	LoadAllMaterial();
	LoadAllFlipbook();
	LoadAllPrefab();
	LoadAllLevel();
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

	pShader->AddShaderParam(SHADER_PARAM::VEC4, 0, L"TintColor");
	pShader->AddShaderParam(SHADER_PARAM::TEX, 0, L"OutColor");

	AddAsset(L"Std2DShader", pShader.Get());


	// ============== BillboardShader 생성 ==============//
	pShader = new AGraphicShader;
	pShader->SetName(L"BillboardShader");
	pShader->CreateVertexShader(L"Shader\\billboard.fx", "VS_Billboard");
	pShader->CreatePixelShader(L"Shader\\billboard.fx", "PS_Billboard");
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());

	// ============== SpriteShader 생성 ==============//
	pShader = new AGraphicShader;
	pShader->SetName(L"SpriteShader");
	pShader->CreateVertexShader(L"Shader\\sprite.fx", "VS_Sprite");
	pShader->CreatePixelShader(L"Shader\\sprite.fx", "PS_Sprite");
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());

	// ============== FlipbookShader 생성 ==============//
	pShader = new AGraphicShader;
	pShader->SetName(L"FlipbookShader");
	pShader->CreateVertexShader(L"Shader\\flipbook.fx", "VS_Flipbook");
	pShader->CreatePixelShader(L"Shader\\flipbook.fx", "PS_Flipbook");
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());

	// ============== TileShader 생성 ==============//
	pShader = new AGraphicShader;
	pShader->SetName(L"TileShader");
	pShader->CreateVertexShader(L"Shader\\tile.fx", "VS_Tile");
	pShader->CreatePixelShader(L"Shader\\tile.fx", "PS_Tile");
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());


	// ============= BGScrollShader ================//
	pShader = new AGraphicShader;
	pShader->SetName(L"BGScrollShader");
	pShader->CreateVertexShader(L"Shader\\bg_scroll.fx", "VS_BGScroll2D");
	pShader->CreatePixelShader(L"Shader\\bg_scroll.fx", "PS_BGScroll2D");
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->AddShaderParam(SHADER_PARAM::FLOAT, 0, L"Speed");
	pShader->AddShaderParam(SHADER_PARAM::TEX, 0, L"OutColor");

	AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());

	// ============== EffectShader 생성 ==============//
	pShader = new AGraphicShader;
	pShader->SetName(L"EffectShader");
	pShader->CreateVertexShader(L"Shader\\effect.fx", "VS_Effect");
	pShader->CreatePixelShader(L"Shader\\effect.fx", "PS_Effect");
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS_EQUL);

	pShader->AddShaderParam(SHADER_PARAM::FLOAT, 0, L"AlphaValue");
	pShader->AddShaderParam(SHADER_PARAM::TEX, 0, L"OutColor");

	AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());

	// ============== ButtonShader 생성 ==============//
	pShader = new AGraphicShader;
	pShader->SetName(L"ButtonShader");
	pShader->CreateVertexShader(L"Shader\\Button.fx", "VS_Button");
	pShader->CreatePixelShader(L"Shader\\Button.fx", "PS_Button");
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->AddShaderParam(SHADER_PARAM::INT, 0, L"ButtonState");

	pShader->AddShaderParam(SHADER_PARAM::VEC4, 0, L"TintColor");

	pShader->AddShaderParam(SHADER_PARAM::TEX, 0, L"NormalTex");
	pShader->AddShaderParam(SHADER_PARAM::TEX, 1, L"PressedTex");

	AssetMgr::GetInst()->AddAsset(pShader->GetName(), pShader.Get());

	// ============== AlphaShader 생성 ==============//
	pShader = new AGraphicShader;
	pShader->SetName(L"AlphaShader");
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D");
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);

	pShader->AddShaderParam(SHADER_PARAM::VEC4, 0, L"TintColor");
	pShader->AddShaderParam(SHADER_PARAM::TEX, 0, L"OutColor");

	AddAsset(L"AlphaShader", pShader.Get());

	// ============== BackAlphaShader 생성 ==============//
	pShader = new AGraphicShader;
	pShader->SetName(L"BackAlphaShader");
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D");
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS_EQUL);

	pShader->AddShaderParam(SHADER_PARAM::VEC4, 0, L"TintColor");
	pShader->AddShaderParam(SHADER_PARAM::TEX, 0, L"OutColor");

	AddAsset(L"BackAlphaShader", pShader.Get());

	// ============== HPBarShader 생성 ==============//
	pShader = new AGraphicShader;
	pShader->SetName(L"ButtonShader");
	pShader->CreateVertexShader(L"Shader\\HPBar.fx", "VS_HPBar");
	pShader->CreatePixelShader(L"Shader\\HPBar.fx", "PS_HPBar");
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	AddAsset(L"HPBarShader", pShader.Get());
	
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
	Load<ATexture>(L"Missile", L"Texture\\Missile.png");
	Load<ATexture>(L"Enemy", L"Texture\\Enemy.png");
	Load<ATexture>(L"Link", L"Texture\\link.png");
	Load<ATexture>(L"TileAtlas", L"Texture\\TILE.bmp");
	Load<ATexture>(L"Cookie", L"Texture\\Cookie.png");

	// ==============================
	// 젤리 텍스쳐
	// ==============================
	Load<ATexture>(L"JellyDefault", L"Texture\\Jelly\\jellybean_31.png");
	Load<ATexture>(L"JellyCoin_1", L"Texture\\Jelly\\JellyCoin_1.png");
	Load<ATexture>(L"JellyCoin_2", L"Texture\\Jelly\\JellyCoin_2.png");
	Load<ATexture>(L"Jelly_bearyellow_z65", L"Texture\\Jelly\\jelly_bearyellow_z65.png");
	Load<ATexture>(L"Jelly_bearpink_z65", L"Texture\\Jelly\\jelly_bearpink_z65.png");
	Load<ATexture>(L"Jelly_bearice_z65", L"Texture\\Jelly\\jelly_bearice_z65.png");
	Load<ATexture>(L"Jelly_BearBig", L"Texture\\Jelly\\Jelly_BearBig.png");
	Load<ATexture>(L"Jelly_BearRainbow_z65", L"Texture\\Jelly\\Jelly_BearRainbow_z65.png");
	Load<ATexture>(L"Jelly_BearRainbow_z65", L"Texture\\Jelly\\Jelly_BearRainbow_z65.png");
	Load<ATexture>(L"Jelly_SpecialBonus_1", L"Texture\\Jelly\\Jelly_SpecialBonus_1.png");
	Load<ATexture>(L"Jelly_SpecialBonus_2", L"Texture\\Jelly\\Jelly_SpecialBonus_2.png");




	// ==============================
	// 리치드래곤 쿠키 애니메이션 텍스쳐
	// ==============================
	Load<ATexture>(L"DragonCookieIdle", L"Texture\\Lychee\\Lychee_Idle.png");
	Load<ATexture>(L"DragonCookieJump", L"Texture\\Lychee\\Lychee_Jump.png");
	Load<ATexture>(L"DragonCookieDoubleJump", L"Texture\\Lychee\\Lychee_DoubleJump.png");
	Load<ATexture>(L"DragonCookieLand", L"Texture\\Lychee\\Lychee_Land.png");
	Load<ATexture>(L"DragonCookieHit", L"Texture\\Lychee\\Lychee_Hit.png");
	Load<ATexture>(L"DragonCookieSlide", L"Texture\\Lychee\\Lychee_Slide.png");
	Load<ATexture>(L"DragonCookieDead", L"Texture\\Lychee\\Lychee_Dead.png");
	Load<ATexture>(L"DragonCookieHitDead", L"Texture\\Lychee\\Lychee_Hit_Dead.png");
	Load<ATexture>(L"DragonCookieSkillStart", L"Texture\\Lychee\\Lychee_SkillStart.png");
	Load<ATexture>(L"DragonCookieSkillSlide", L"Texture\\Lychee\\Lychee_SkillSlide.png");
	Load<ATexture>(L"DragonCookieSkillEnd", L"Texture\\Lychee\\Lychee_SkillEnd.png");
	Load<ATexture>(L"DragonCookieBonusTime_Down", L"Texture\\Lychee\\Lychee_BonusTime_Down.png");
	Load<ATexture>(L"DragonCookieBonusTime_Up", L"Texture\\Lychee\\Lychee_BonusTime_Up.png");
	Load<ATexture>(L"DragonCookieBoost", L"Texture\\Lychee\\Lychee_Boost.png");

	Load<ATexture>(L"DragonCookie_BG", L"Texture\\Lychee\\Lychee_BG.png");

	Load<ATexture>(L"DragonCookie_SkillBG", L"Texture\\Lychee\\Lychee_SkillBG.png");
	Load<ATexture>(L"DragonCookie_SkillEffect_0", L"Texture\\Lychee\\Lychee_SkillEffect_0.png");
	Load<ATexture>(L"DragonCookie_SkillEffect_1", L"Texture\\Lychee\\Lychee_SkillEffect_1.png");

	// ==============================
	// 시간지기 쿠키 애니메이션 텍스쳐
	// ==============================
	Load<ATexture>(L"TimeKeeperCookieIdle", L"Texture\\TimeKeeper\\Timekeeper_Idle.png");
	Load<ATexture>(L"TimeKeeperCookieJump", L"Texture\\TimeKeeper\\Timekeeper_Jump.png");
	Load<ATexture>(L"TimeKeeperCookieDoubleJump", L"Texture\\TimeKeeper\\TimeKeeper_DoubleJump.png");
	Load<ATexture>(L"TimeKeeperCookieLand", L"Texture\\TimeKeeper\\TimeKeeper_Land.png");
	Load<ATexture>(L"TimeKeeperCookieHit", L"Texture\\TimeKeeper\\TimeKeeper_Hit.png");
	Load<ATexture>(L"TimeKeeperCookieSlide", L"Texture\\TimeKeeper\\TimeKeeper_Slide.png");
	Load<ATexture>(L"TimeKeeperCookieDead", L"Texture\\TimeKeeper\\TimeKeeper_Dead.png");
	Load<ATexture>(L"TimeKeeperCookieSkillStart", L"Texture\\TimeKeeper\\TimeKeeper_SkillStart.png");
	Load<ATexture>(L"TimeKeeperCookieSkill", L"Texture\\TimeKeeper\\TimeKeeper_Skill.png");
	Load<ATexture>(L"TimeKeeperCookieSkillSlide", L"Texture\\TimeKeeper\\TimeKeeper_SkillSlide.png");
	Load<ATexture>(L"TimeKeeperCookieSkillEnd", L"Texture\\TimeKeeper\\TimeKeeper_SkillEnd.png");
	Load<ATexture>(L"TimeKeeperCookieBonusTimeDown", L"Texture\\TimeKeeper\\TimeKeeper_BonusTime_Down.png");
	Load<ATexture>(L"TimeKeeperCookieBonusTimeUp", L"Texture\\TimeKeeper\\TimeKeeper_BonusTime_Up.png");
	Load<ATexture>(L"TimeKeeperCookieBonusTimeUp", L"Texture\\TimeKeeper\\TimeKeeper_BonusTime_Up.png");
	Load<ATexture>(L"TimeKeeperBoost", L"Texture\\TimeKeeper\\TimeKeeper_Boost.png");
	
	
	Load<ATexture>(L"TimeKeeper_bg1", L"Texture\\TimeKeeper\\TimeKeeper_bg1.png");
	Load<ATexture>(L"TimeKeeper_bg2", L"Texture\\TimeKeeper\\TimeKeeper_bg2.png");
	Load<ATexture>(L"TimeKeeper_bg3", L"Texture\\TimeKeeper\\TimeKeeper_bg3.png");

	Load<ATexture>(L"TimeKeeper_SkillBG", L"Texture\\TimeKeeper\\TimeKeeper_SkillBG.png");
	Load<ATexture>(L"TimeKeeper_SkillEffect_0", L"Texture\\TimeKeeper\\TimeKeeper_SkillEffect_0.png");
	Load<ATexture>(L"TimeKeeper_SkillEffect_1_1", L"Texture\\TimeKeeper\\TimeKeeper_SkillEffect_1_1.png");
	Load<ATexture>(L"TimeKeeper_SkillEffect_1_2", L"Texture\\TimeKeeper\\TimeKeeper_SkillEffect_1_2.png");
	Load<ATexture>(L"TimeKeeper_SkillEffect_2_1", L"Texture\\TimeKeeper\\TimeKeeper_SkillEffect_2_1.png");
	Load<ATexture>(L"TimeKeeper_SkillEffect_2_2", L"Texture\\TimeKeeper\\TimeKeeper_SkillEffect_2_2.png");
	Load<ATexture>(L"TimeKeeper_SkillEffect_3_1", L"Texture\\TimeKeeper\\TimeKeeper_SkillEffect_3_1.png");
	Load<ATexture>(L"TimeKeeper_SkillEffect_3_2", L"Texture\\TimeKeeper\\TimeKeeper_SkillEffect_3_2.png");


	// ========
	// 펫
	// ========
	Load<ATexture>(L"Pet_Default", L"Texture\\Pet\\Pet_Default.png");
	Load<ATexture>(L"DragonCookiePetIdle", L"Texture\\Pet\\Pet_Lycheye.png");
	Load<ATexture>(L"Pet_TimeKeeper", L"Texture\\Pet\\Pet_TimeKeeper.png");


	// ==========
	// 해적 맵_1 텍스쳐
	// ==========
	Load<ATexture>(L"BackGroundShipStage1_1", L"Texture\\ovenbreak_tm001_bg1.png");
	Load<ATexture>(L"BackGroundShipStage1_2", L"Texture\\ovenbreak_tm001_bg2.png");
	Load<ATexture>(L"TileShipStage1_1", L"Texture\\ovenbreak_tm001_tb.png");
	Load<ATexture>(L"TileShipStage1_2", L"Texture\\ovenbreak_tm001_fh.png");


	// ==============
	// 이펙트 텍스쳐
	// ==============
	Load<ATexture>(L"Effect_JellyCollect", L"Texture\\Effect\\Effect_JellyCollect.png");
	Load<ATexture>(L"Effect_CookieBoost", L"Texture\\Effect\\Effect_CookieBoost.png");
	Load<ATexture>(L"Effect_Obstruct", L"Texture\\Obstacle\\Obstruct.png");
	Load<ATexture>(L"Effect_Hit", L"Texture\\Effect\\Effect_Hit.png");





	// ==============================
	// 장애물 텍스쳐
	// ==============================
	Load<ATexture>(L"Obstacle_bl1_crash", L"Texture\\Obstacle\\bl1_crash.png");
	Load<ATexture>(L"Obstacle_bl1_ing", L"Texture\\Obstacle\\bl1_ing.png");
	Load<ATexture>(L"Obstacle_cb2_open", L"Texture\\Obstacle\\cb2_open.png");
	Load<ATexture>(L"Obstacle_tm001_noti", L"Texture\\Obstacle\\tm001_noti.png");
	Load<ATexture>(L"Obstacle_tm001_ing", L"Texture\\Obstacle\\tm001_ing.png");
	Load<ATexture>(L"Obstacle_jp1down", L"Texture\\Obstacle\\jp1down.png");
	Load<ATexture>(L"Obstacle_jp1down_ing", L"Texture\\Obstacle\\jp1down_ing.png");
	Load<ATexture>(L"Obstacle_tm001_sdswing", L"Texture\\Obstacle\\tm001_sdswing.png");
	Load<ATexture>(L"Obstacle_cb2_open", L"Texture\\Obstacle\\cb2_open.png");
	Load<ATexture>(L"Obstacle_tm001_jp1A", L"Texture\\Obstacle\\tm001_jp1A.png");
	Load<ATexture>(L"Obstacle_tm001_jp1B", L"Texture\\Obstacle\\tm001_jp1B.png");
	Load<ATexture>(L"Obstacle_tm001_sdA", L"Texture\\Obstacle\\tm001_sdA.png");


	// ==============================
	// 아이템 텍스쳐
	// ==============================
	Load<ATexture>(L"ItemBoost", L"Texture\\Item\\ItemBoost.png");
	Load<ATexture>(L"ItemGiant", L"Texture\\Item\\ItemGiant.png");
	Load<ATexture>(L"ItemHeal_A", L"Texture\\Item\\ItemHeal_A.png");
	Load<ATexture>(L"ItemHeal_B", L"Texture\\Item\\ItemHeal_B.png");
	Load<ATexture>(L"ItemMagnet", L"Texture\\Item\\ItemMagnet.png");

	Load<ATexture>(L"ImgTxt_ingameBoost", L"Texture\\Item\\imgTxt_ingameBoost.png");
	Load<ATexture>(L"ImgTxt_ingameGiant", L"Texture\\Item\\imgTxt_ingameGiant.png");
	Load<ATexture>(L"ImgTxt_ingameHeal", L"Texture\\Item\\imgTxt_ingameHeal.png");
	Load<ATexture>(L"ImgTxt_ingameMagnet", L"Texture\\Item\\imgTxt_ingameMagnet.png");


	// ==============================
	// UI 텍스쳐
	// ==============================
	Load<ATexture>(L"BlackPanel", L"Texture\\UI\\BlackPanel.png");

	Load<ATexture>(L"btn_jump", L"Texture\\UI\\btn_jump.png");
	Load<ATexture>(L"btn_jump_dim", L"Texture\\UI\\btn_jump_dim.png");
	Load<ATexture>(L"btn_slide", L"Texture\\UI\\btn_slide.png");
	Load<ATexture>(L"btn_slide_dim", L"Texture\\UI\\btn_slide_dim.png");

	Load<ATexture>(L"btn_puase_no", L"Texture\\UI\\btn_puase_no.png");

	Load<ATexture>(L"Icon_heartLife", L"Texture\\UI\\Icon_heartLife.png");
	Load<ATexture>(L"Icon_heart_orange", L"Texture\\UI\\Icon_heart_orange.png");
	Load<ATexture>(L"Icon_heart_green", L"Texture\\UI\\Icon_heart_green.png");
	Load<ATexture>(L"Icon_heart_blue", L"Texture\\UI\\Icon_heart_blue.png");
	Load<ATexture>(L"Icon_heartLife_bg", L"Texture\\UI\\gaugebg_heart01.png");
	
	Load<ATexture>(L"Icon_jellybean0001_big", L"Texture\\UI\\jellybean0001_big.png");

	Load<ATexture>(L"btn_blue", L"Texture\\UI\\btn_burningRelay.png");
	Load<ATexture>(L"btn_green", L"Texture\\UI\\btn_useRelay.png");
	Load<ATexture>(L"btn_gray", L"Texture\\UI\\btn_gray.png");
	Load<ATexture>(L"btn_yellow", L"Texture\\UI\\btn_yellow.png");


	Load<ATexture>(L"SelectIcon_TimeKeeper", L"Texture\\UI\\SelectIcon_TimeKeeper.png");
	Load<ATexture>(L"SelectIcon_Dragon", L"Texture\\UI\\SelectIcon_Dragon.png");


	Load<ATexture>(L"Score_BG", L"Texture\\UI\\Score_BG.png");
	Load<ATexture>(L"Panel_Ribbon", L"Texture\\UI\\img_burningDay_ingameRibbon.png");

	// =========== 로비 UI 텍스쳐 ============
	Load<ATexture>(L"bg_lobby_skin", L"Texture\\Lobby\\bg_lobby_skin1.png");
	Load<ATexture>(L"icon_ingame_level", L"Texture\\Lobby\\icon_ingame_chapter02.png");
	Load<ATexture>(L"icon_ingame_relayCoin", L"Texture\\Lobby\\icon_ingame_relayCoin.png");
	Load<ATexture>(L"icon_ingame_rescueCristal", L"Texture\\Lobby\\icon_ingame_rescueCristal.png");
	Load<ATexture>(L"UI_ingame_ovenBreakGauge", L"Texture\\Lobby\\imf_ingame_ovenBreakGauge.png");
	Load<ATexture>(L"UI_ingame_blackBox", L"Texture\\Lobby\\frmBg_ingameTreasure.png");


	// == 사운드 ==
	Load<ASound>(L"Lobby_Sound", L"Sound\\Lobby_Sound.mp3");
	Load<ASound>(L"GamePlay_Sound", L"Sound\\GamePlay_Sound.mp3");
	Load<ASound>(L"r_score", L"Sound\\r_score.ogg");

	Load<ASound>(L"DragonCookie_Skill", L"Sound\\DragonCookie_Skill.mp3");
	Load<ASound>(L"TimeKeeperCookie_Skill", L"Sound\\TimeKeeperCookie_Skill.mp3");
	Load<ASound>(L"slide", L"Sound\\slide.ogg");
	Load<ASound>(L"jump", L"Sound\\jump.ogg");
	Load<ASound>(L"hit", L"Sound\\hit.ogg");
	Load<ASound>(L"dead", L"Sound\\dead.ogg");

	Load<ASound>(L"g_gold", L"Sound\\g_gold.ogg");
	Load<ASound>(L"g_ijelly", L"Sound\\g_ijelly.ogg");
	Load<ASound>(L"g_jelly", L"Sound\\g_jelly.ogg");
	Load<ASound>(L"breakObs", L"Sound\\breakObs.ogg");

	Load<ASound>(L"i_giant", L"Sound\\i_giant.ogg");
	Load<ASound>(L"i_large_energy", L"Sound\\i_large_energy.ogg");
	Load<ASound>(L"i_magnet", L"Sound\\i_magnet.ogg");

	Load<ASound>(L"ui_1", L"Sound\\ui_1.ogg");
}

void AssetMgr::CreateEngineMaterial()
{
	// 재질 만들기
	Ptr<AMaterial> pMtrl = nullptr;

	//================== Std2DMtrl 생성 ==============//
	pMtrl = new AMaterial;
	pMtrl->SetName(L"Std2DMtrl");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"Std2DShader"));

	// Parameter
	pMtrl->SetScalar(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
	pMtrl->SetTexture(TEX_0, FindAsset<ATexture>(L"PlayerImage"));

	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	//================== MonsterMtrl 생성 ==============//
	pMtrl = new AMaterial;
	pMtrl->SetName(L"MonsterMtrl");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"Std2DShader"));

	// Parameter
	pMtrl->SetScalar(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
	pMtrl->SetTexture(TEX_0, FindAsset<ATexture>(L"Enemy"));

	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	//================== EffectMtrl 생성 ==============//
	pMtrl = new AMaterial;
	pMtrl->SetName(L"EffectMtrl");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"EffectShader"));

	// Parameter
	pMtrl->SetScalar(FLOAT_0, 1.0f);
	pMtrl->SetTexture(TEX_0, FindAsset<ATexture>(L"Effect_JellyCollect"));

	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	// ================== 해적 맵_1 생성 ==============//
	pMtrl = new AMaterial;
	pMtrl->SetName(L"BackGround_Ship_Stage1_1_Mtrl");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"BGScrollShader"));

	// Parameter
	pMtrl->SetScalar(VEC2_0, (0.f, 0.f));
	pMtrl->SetTexture(TEX_0, FindAsset<ATexture>(L"BackGroundShipStage1_1"));

	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_OPAQUE);
	AddAsset(pMtrl->GetName(), pMtrl.Get());


	pMtrl = new AMaterial;
	pMtrl->SetName(L"BackGround_Ship_Stage1_2_Mtrl");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"BGScrollShader"));

	// Parameter
	pMtrl->SetScalar(VEC2_0, (0.f, 0.f));
	pMtrl->SetTexture(TEX_0, FindAsset<ATexture>(L"BackGroundShipStage1_2"));

	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_OPAQUE);
	AddAsset(pMtrl->GetName(), pMtrl.Get());



	pMtrl = new AMaterial;
	pMtrl->SetName(L"Tile_Ship_Stage1_1_Mtrl");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"Std2DShader"));

	// Parameter
	pMtrl->SetScalar(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
	pMtrl->SetTexture(TEX_0, FindAsset<ATexture>(L"TileShipStage1_1"));

	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_OPAQUE);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	pMtrl = new AMaterial;
	pMtrl->SetName(L"Tile_Ship_Stage1_2_Mtrl");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"Std2DShader"));

	// Parameter
	pMtrl->SetScalar(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
	pMtrl->SetTexture(TEX_0, FindAsset<ATexture>(L"TileShipStage1_2"));

	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_OPAQUE);
	AddAsset(pMtrl->GetName(), pMtrl.Get());


	// ================== AlphaMtrl 생성 ==============//
	pMtrl = new AMaterial;
	pMtrl->SetName(L"UI_AlphaPanel");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"AlphaShader"));

	pMtrl->SetScalar(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
	pMtrl->SetTexture(TEX_0, FindAsset<ATexture>(L"UI_ingame_ovenBreakGauge"));

	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	// ================== UIButtonMtrl 생성 ==============//
	pMtrl = new AMaterial;
	pMtrl->SetName(L"UIButtonMtrl");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"ButtonShader"));

	// 상태
	pMtrl->SetScalar(INT_0, 0); // Normal

	// Tint
	pMtrl->SetScalar(VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));

	// 텍스처 2개만 사용
	pMtrl->SetTexture(TEX_0, FindAsset<ATexture>(L"btn_jump"));
	pMtrl->SetTexture(TEX_1, FindAsset<ATexture>(L"btn_jump_dim"));

	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(pMtrl->GetName(), pMtrl.Get());

	// ================== HPBarMtrl 생성 ==============//
	pMtrl = new AMaterial;
	pMtrl->SetName(L"HPBarMtrl");
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"HPBarShader"));

	pMtrl->SetScalar(FLOAT_0, 1.0f); // 처음에는 가득 찬 상태
	pMtrl->SetTexture(TEX_0, FindAsset<ATexture>(L"Icon_heart_orange"));

	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	AddAsset(pMtrl->GetName(), pMtrl.Get());

	//================== DbgMtrl 생성 ==============//
	pMtrl = new AMaterial;
	pMtrl->SetName(L"DbgMtrl");
	pMtrl->SetDomain(RENDER_DOMAIN::DOMAIN_MASKED);
	pMtrl->SetShader(FindAsset<AGraphicShader>(L"DbgShader"));


	AddAsset(pMtrl->GetName(), pMtrl.Get());
}

void AssetMgr::CreateEngineSprite()
{


	/*
	Ptr<ATexture> pAtlas = FIND(ATexture, L"Link");
	float Width = pAtlas->GetWidth();
	float Height = pAtlas->GetHeight();
	Vec2 SlicePixel = Vec2(120.f, 130.f);
	
	Ptr<ASprite> pSprite = nullptr;
	for (int i = 0; i < 10; i++)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\LinkMD_%d.sprite", i);

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
		swprintf_s(Buff, L"Sprite\\LinkML_%d.sprite", i);

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
		swprintf_s(Buff, L"Sprite\\LinkMU_%d.sprite", i);

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
		swprintf_s(Buff, L"Sprite\\LinkMR_%d.sprite", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 7.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}
	*/


	/*
	Ptr<AFlipbook> pFlipbook = nullptr;
	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Flipbook\\Link_MoveDown.flip");

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\LinkMD_%d.sprite", i);
		pFlipbook->AddSprite(LOAD(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Flipbook\\Link_MoveLeft.flip");
	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\LinkML_%d.sprite", i);
		pFlipbook->AddSprite(LOAD(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Flipbook\\Link_MoveUp.flip");

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\LinkMU_%d.sprite", i);
		pFlipbook->AddSprite(LOAD(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());

	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Flipbook\\Link_MoveRight.flip");

	for (int i = 0; i < 10; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\LinkMR_%d.sprite", i);
		pFlipbook->AddSprite(LOAD(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());

	*/

	//============================================

	/*
	pAtlas = FIND(ATexture, L"DragonCookieIdle");
	Width = pAtlas->GetWidth();
	Height = pAtlas->GetHeight();
	SlicePixel = Vec2(364.f, 364.f);

	pSprite = nullptr;
	for (int i = 0; i < 4; i++)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\DragonCookieIdle_%d.sprite", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 0.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}

	pAtlas = FIND(ATexture, L"DragonCookieJump");
	Width = pAtlas->GetWidth();
	Height = pAtlas->GetHeight();
	SlicePixel = Vec2(364.f, 364.f);

	pSprite = nullptr;
	for (int i = 0; i < 2; i++)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\DragonCookieJump_%d.sprite", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 0.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}


	pAtlas = FIND(ATexture, L"DragonCookieDoubleJump");
	Width = pAtlas->GetWidth();
	Height = pAtlas->GetHeight();
	SlicePixel = Vec2(364.f, 364.f);

	pSprite = nullptr;
	for (int i = 0; i < 5; i++)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\DragonCookieDoubleJump_%d.sprite", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 0.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}

	pAtlas = FIND(ATexture, L"DragonCookiePetIdle");
	Width = pAtlas->GetWidth();
	Height = pAtlas->GetHeight();
	SlicePixel = Vec2(273.f, 273.f);

	pSprite = nullptr;
	for (int i = 0; i < 5; i++)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\DragonCookiePetIdle_%d.sprite", i);

		pSprite = new ASprite;
		pSprite->SetName(Buff);
		pSprite->SetAtlas(pAtlas);
		pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 0.f));
		pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
		AddAsset(pSprite->GetName(), pSprite.Get());
	}

	*/

	/*
	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Flipbook\\DragonCookie_Idle.flip");

	for (int i = 0; i < 4; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\DragonCookieIdle_%d.sprite", i);
		pFlipbook->AddSprite(LOAD(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());


	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Flipbook\\DragonCookie_Jump.flip");

	for (int i = 0; i < 2; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\DragonCookieJump_%d.sprite", i);
		pFlipbook->AddSprite(LOAD(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());


	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Flipbook\\DragonCookie_DoubleJump.flip");

	for (int i = 0; i < 5; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\DragonCookieDoubleJump_%d.sprite", i);
		pFlipbook->AddSprite(LOAD(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());


	pFlipbook = new AFlipbook;
	pFlipbook->SetName(L"Flipbook\\DragonCookie_PetIdle.flip");

	for (int i = 0; i < 5; ++i)
	{
		wchar_t Buff[50] = {};
		swprintf_s(Buff, L"Sprite\\DragonCookiePetIdle_%d.sprite", i);
		pFlipbook->AddSprite(LOAD(ASprite, Buff));
	}
	AddAsset(pFlipbook->GetName(), pFlipbook.Get());
	*/

 //추가할때 임시
	//Ptr<ATexture> pAtlas = FIND(ATexture, L"TimeKeeperCookieIdle");
	//float Width = pAtlas->GetWidth();
	//float Height = pAtlas->GetHeight();
	//Vec2 SlicePixel = Vec2(359.f, 359.f);
	//
	//Ptr<ASprite> pSprite = nullptr;
	//for (int i = 0; i < 4; i++)
	//{
	//	wchar_t Buff[50] = {};
	//	swprintf_s(Buff, L"Sprite\\TimeKeeperCookieIdle_%d.sprite", i);
	//
	//	pSprite = new ASprite;
	//	pSprite->SetName(Buff);
	//	pSprite->SetAtlas(pAtlas);
	//	pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)i, (SlicePixel.y / Height) * 0.f));
	//	pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
	//	AddAsset(pSprite->GetName(), pSprite.Get());
	//}

	//Ptr<AFlipbook> pFlipbook = new AFlipbook;
	//pFlipbook->SetName(L"Flipbook\\TimeKeeperCookie_Idle.flip");
	//
	//for (int i = 0; i < 4; ++i)
	//{
	//	wchar_t Buff[50] = {};
	//	swprintf_s(Buff, L"Sprite\\TimeKeeperCookieIdle_%d.sprite", i);
	//	pFlipbook->AddSprite(LOAD(ASprite, Buff));
	//}
	//AddAsset(pFlipbook->GetName(), pFlipbook.Get());


	// ============
	// Tile Sprite
	// ============
	
	/* pAtlas = FIND(ATexture, L"TileAtlas");

	Width = pAtlas->GetWidth();
	Height = pAtlas->GetHeight();
	SlicePixel = Vec2(64.f, 64.f);

	int Count = 0;
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 8; ++j, ++Count)
		{
			wchar_t Buff[50] = {};
			swprintf_s(Buff, L"Sprite\\TileSprite_%d.sprite", Count);

			pSprite = new ASprite;
			pSprite->SetName(Buff);
			pSprite->SetAtlas(pAtlas);
			pSprite->SetLeftTopUV(Vec2((SlicePixel.x / Width) * (float)j, (SlicePixel.y / Height) * i));
			pSprite->SetSliceUV(SlicePixel / Vec2(Width, Height));
			AddAsset(pSprite->GetName(), pSprite.Get());
		}
	}
	*/

	 //========
	 //TileMap
	 //========
	//Ptr<ATileMap> pTileMap = nullptr;

	//pTileMap = new ATileMap;
	//pTileMap->SetName(L"TileMap\\TestTileMap");
	//pTileMap->SetRowCol(20, 20);
	//pTileMap->SetTileSize(Vec2(64.f, 64.f));
	//pTileMap->SetAtlas(FIND(ATexture, L"TileAtlas"));

	//for (int i = 0; i < 20; ++i)
	//	for (int j = 0; j < 20; ++j)
	//		pTileMap->SetSprite(i, j, LOAD(ASprite, L"Sprite\\TileSprite_1.sprite"));

	//AddAsset(pTileMap->GetName(), pTileMap.Get());
	//pTileMap->Save(CONTENT_PATH + pTileMap->GetKey());

	// 모든 스프라이트를 파일로 저장
	//for (const auto& pair : m_mapAsset[(UINT)ASSET_TYPE::SPRITE])
	//{
	//	wstring FilePath = CONTENT_PATH + pair.first;
	//	pair.second->Save(FilePath);
	//}

	//모든 Flipbook을 파일로 저장
	//for (const auto& pair : m_mapAsset[(UINT)ASSET_TYPE::FLIPBOOK])
	//{
	//	wstring FilePath = CONTENT_PATH + pair.first;
	//	pair.second->Save(FilePath);
	//}
}

#include "Source/Scripts/CMissileScript.h"
void AssetMgr::CreateEnginePrefab()
{
	//// Missile 역할 프리팹 제작
	//GameObject* pObject = new GameObject;
	//pObject->SetName(L"Missile");
	//
	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);
	//pObject->AddComponent(new CCollider2D);	
	//pObject->AddComponent(new CMissileScript);		
	//
	//pObject->Transform()->SetRelativeScale(Vec3(10.f, 30.f, 1.f));
	//
	//pObject->MeshRender()->SetMesh(AssetMgr::GetInst()->FindAsset<AMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(AssetMgr::GetInst()->FindAsset<AMaterial>(L"Std2DMtrl"));
	//
	//// 미사일 오브젝트를 프리팹으로 등록
	//Ptr<APrefab> pMissilePrefab = new APrefab;
	//pMissilePrefab->SetObject(pObject);
	//AddAsset(L"Prefab\\Missile.pref", pMissilePrefab.Get());
	//
	//wstring FilePath = CONTENT_PATH + L"Prefab\\Missile.pref";
	//pMissilePrefab->Save(FilePath);
}

void AssetMgr::LoadAllSprite()
{
	wstring spritePath = CONTENT_PATH + L"Sprite\\";

	if (!std::filesystem::exists(spritePath))
		return;

	for (const auto& pair : std::filesystem::directory_iterator(spritePath))
	{
		if (pair.is_regular_file() && pair.path().extension() == L".sprite")
		{
			wstring fileName = L"Sprite\\" + pair.path().filename().wstring();
			wstring relativePath = L"Sprite\\" + pair.path().filename().wstring();

			if (FindAsset<ASprite>(fileName) == nullptr)
			{
				Ptr<ASprite> pSprite = new ASprite;
				if (S_OK == pSprite->Load(pair.path().wstring()))
				{
					AddAsset(fileName, pSprite.Get());
				}
			}
		}
	}
}

void AssetMgr::LoadAllMaterial()
{
	wstring MaterialPath = CONTENT_PATH + L"Material\\";

	if (!std::filesystem::exists(MaterialPath))
		return;

	for (const auto& pair : std::filesystem::directory_iterator(MaterialPath))
	{
		if (pair.is_regular_file() && pair.path().extension() == L".mtrl")
		{
			wstring fileName = L"Material\\" + pair.path().filename().wstring();
			wstring relativePath = L"Material\\" + pair.path().filename().wstring();

			if (FindAsset<AMaterial>(fileName) == nullptr)
			{
				Ptr<AMaterial> pMtrl = new AMaterial;
				if (S_OK == pMtrl->Load(pair.path().wstring()))
				{
					AddAsset(fileName, pMtrl.Get());
				}
			}
		}
	}
}

void AssetMgr::LoadAllFlipbook()
{
	wstring FlipBookPath = CONTENT_PATH + L"Flipbook\\";

	if (!std::filesystem::exists(FlipBookPath))
		return;

	for (const auto& pair : std::filesystem::directory_iterator(FlipBookPath))
	{
		if (pair.is_regular_file() && pair.path().extension() == L".flip")
		{
			wstring fileName = L"Flipbook\\" + pair.path().filename().wstring();
			wstring relativePath = L"Flipbook\\" + pair.path().filename().wstring();

			if (FindAsset<AFlipbook>(fileName) == nullptr)
			{
				Ptr<AFlipbook> pFlipbook = new AFlipbook;
				if (S_OK == pFlipbook->Load(pair.path().wstring()))
				{
					AddAsset(fileName, pFlipbook.Get());
				}
			}
		}
	}
}

void AssetMgr::LoadAllPrefab()
{
	wstring PrefabPath = CONTENT_PATH + L"Prefab\\";

	if (!std::filesystem::exists(PrefabPath))
		return;

	for (const auto& pair : std::filesystem::directory_iterator(PrefabPath))
	{
		if (pair.is_regular_file() && pair.path().extension() == L".pref")
		{
			wstring fileName = L"Prefab\\" + pair.path().filename().wstring();
			wstring relativePath = L"Prefab\\" + pair.path().filename().wstring();

			if (FindAsset<APrefab>(fileName) == nullptr)
			{
				Ptr<APrefab> pPrefab = new APrefab;
				if (S_OK == pPrefab->Load(pair.path().wstring()))
				{
					AddAsset(fileName, pPrefab.Get());
				}
			}
		}
	}
}

void AssetMgr::LoadAllLevel()
{
	wstring LevelPath = CONTENT_PATH + L"Level\\";

	if (!std::filesystem::exists(LevelPath))
		return;

	for (const auto& pair : std::filesystem::directory_iterator(LevelPath))
	{
		if (pair.is_regular_file() && pair.path().extension() == L".lv")
		{
			wstring fileName = L"Level\\" + pair.path().filename().wstring();
			wstring relativePath = L"Level\\" + pair.path().filename().wstring();

			if (FindAsset<ALevel>(fileName) == nullptr)
			{
				Ptr<ALevel> pLevel = new ALevel;
				if (S_OK == pLevel->Load(pair.path().wstring()))
				{
					AddAsset(fileName, pLevel.Get());
				}
			}
		}
	}
}
