#include "pch.h"
#include "GamePlayMgr.h"
#include "ObjectPoolMgr.h"
#include "AssetMgr.h"

#include "..\GameClient\Source\Scripts\CPlayerScript.h"
#include "..\GameClient\Source\Scripts\CCookieSkillScript.h"
#include "..\GameClient\Source\Scripts\CTimeKeeperScript.h"
#include "..\GameClient\Source\Scripts\CLycheeScript.h"

GamePlayMgr::GamePlayMgr()
	: m_PlayerObject(nullptr)
	, m_Score(0)
	, m_ScrollSpeed(300.f)
	, m_RunCoin(0)
	, m_IsPaused(false)
	, m_PausePrevScrollSpeed(300.f)
{
}

GamePlayMgr::~GamePlayMgr()
{

}

void GamePlayMgr::Init()
{
	m_GamePlayUI = nullptr;
	m_PlayerScript = nullptr;
	m_PlayerObject = nullptr;

	m_Score = 0;
	m_RunCoin = 0;
	m_ScrollSpeed = 300.f;	// 기본 스크롤 속도
	m_IsPaused = false;
	m_PausePrevScrollSpeed = 300.f; // 기본 스크롤 속도로 초기화

	// 기본 StageData 생성
	if (nullptr == m_StageData)
	{
		CreateNewStageData();
	}


	// ==============================================
	// 캐릭터 프리팹과 UI 프리팹 로드 및 인스턴스화
	// ==============================================
	// 로비에서 선택된 캐릭터 타입에 따라 프리팹 결정
	m_SeletectedCharacterPrefab = GetPrefabForCharType(m_SelectedCharacterType);
	if (m_SeletectedCharacterPrefab == nullptr)
	{
		m_SelectedCharacterType = ECharacterType::Default;
		m_SeletectedCharacterPrefab = GetPrefabForCharType(m_SelectedCharacterType);
	}

	Ptr<APrefab> pPrefabUI = FIND(APrefab, L"Prefab\\GamePlayUIRoot.pref");
	m_GamePlayUIRootPrefab = pPrefabUI; // UI 루트 프리팹 저장

	Ptr<AStageData> pStageData = LOAD(AStageData, L"StageData\\TestMap_1.stage");
	m_StageData = pStageData; // StageData 저장

	// 오브젝트 풀 초기화
	ObjectPoolMgr::GetInst()->Init();
}

void GamePlayMgr::Clear()
{
	m_GamePlayUI = nullptr;
	m_PlayerScript = nullptr;
	m_SeletectedCharacterPrefab = nullptr;
	m_PlayerObject = nullptr;
	m_Score = 0;
	m_RunCoin = 0;
	m_IsPaused = false;
	m_PausePrevScrollSpeed = 300.f;
}

void GamePlayMgr::CreateNewStageData()
{
	m_StageData = new AStageData;
}


void GamePlayMgr::ApplySkillToPlayer()
{
	if (m_PlayerObject == nullptr)
		return;

	CCookieSkillScript* pSkill = CreateSkillByCharacterType(m_SelectedCharacterType);
	if (pSkill == nullptr)
		return;

	m_PlayerScript->SetSkillScript(pSkill);
}

void GamePlayMgr::PauseGame()
{
	if (m_IsPaused)
		return;

	m_IsPaused = true;
	m_PausePrevScrollSpeed = m_ScrollSpeed;
	m_ScrollSpeed = 0.f; 
}

void GamePlayMgr::ResumeGame()
{
	if (!m_IsPaused)
		return;

	m_IsPaused = false;
	m_ScrollSpeed = m_PausePrevScrollSpeed;
}

void GamePlayMgr::TogglePause()
{
	if (m_IsPaused)
		ResumeGame();
	else
		PauseGame();
}

wstring GamePlayMgr::GetPrefabKey(int _ObjectID)
{
	switch ((EObjectID)_ObjectID)
	{
	case EObjectID::DefaultJelly:	return L"Prefab\\Jelly_Default.pref";
	case EObjectID::Jelly_SilverCoin:			return L"Prefab\\Jelly_SilverCoin.pref";
	case EObjectID::Jelly_GoldCoin:			return L"Prefab\\Jelly_GoldCoin.pref";
	case EObjectID::BearYellow:		return L"Prefab\\Jelly_Bearyellow.pref";
	case EObjectID::BearPink:		return L"Prefab\\Jelly_Bearpink.pref";
	case EObjectID::BearBlue:		return L"Prefab\\Jelly_Bearice.pref";
	case EObjectID::BearBigYellow:	return L"Prefab\\Jelly_BearBig.pref";
	case EObjectID::BearRainbow:	return L"Prefab\\Jelly_BearRainbow.pref";
	case EObjectID::SpecialBonus_1:	return L"Prefab\\Jelly_SpecialBonus_1.pref";
	case EObjectID::SpecialBonus_2:	return L"Prefab\\Jelly_SpecialBonus_2.pref";
	case EObjectID::Item:			return L"Prefab\\Item.pref";
	case EObjectID::Obstacle_bl1:	return L"Prefab\\Obstacle_bl1.pref";
	case EObjectID::Obstacle_cb2:	return L"Prefab\\Obstacle_cb2.pref";
	case EObjectID::Obstacle_tm001:	return L"Prefab\\Obstacle_tm001.pref";
	case EObjectID::Obstacle_jp1down:	return L"Prefab\\Obstacle_jp1down.pref";
	case EObjectID::Obstacle_tm001_sdswing:	return L"Prefab\\Obstacle_tm001_sdswing.pref";
	case EObjectID::Obstacle_bl1_ing:		return L"Prefab\\Obstacle_bl1_ing.pref";
	case EObjectID::Obstacle_tm001_jp1A:	return L"Prefab\\Obstacle_tm001_jp1A.pref";
	case EObjectID::Obstacle_tm001_jp1B:	return L"Prefab\\Obstacle_tm001_jp1B.pref";
	case EObjectID::Obstacle_cb2_open:		return L"Prefab\\Obstacle_cb2_open.pref";
	case EObjectID::Obstacle_tm001_sdA:		return L"Prefab\\Obstacle_tm001_sdA.pref";
	case EObjectID::TileShipeStage_1_fh_m:	return L"Prefab\\TileShipeStage_1_fh_m.pref";
	case EObjectID::TileShipeStage_1_tb_s:	return L"Prefab\\TileShipeStage_1_tb_s.pref";
	case EObjectID::Item_Giant:		return L"Prefab\\Item_Giant.pref";
	case EObjectID::Item_Boost:	return L"Prefab\\Item_Boost.pref";
	case EObjectID::Item_Magnet:		return L"Prefab\\Item_Magnet.pref";
	case EObjectID::Item_HealA:		return L"Prefab\\Item_HealA.pref";
	case EObjectID::Item_HealB:		return L"Prefab\\Item_HealB.pref";
	case EObjectID::TileShipeStage_1_fh_Road:	return L"Prefab\\TileShipeStage_1_fh_Road.pref";
	case EObjectID::TileShipeStage_1_tb_Road:	return L"Prefab\\TileShipeStage_1_tb_Road.pref";
	case EObjectID::Jelly_Jump_Default:	return L"Prefab\\Jelly_Jump_Default.pref";
	case EObjectID::Jelly_Road_Default:	return L"Prefab\\Jelly_Road_Default.pref";
	case EObjectID::Jelly_Heart_Ice:	return L"Prefab\\Jelly_Heart_Ice.pref";
	case EObjectID::Jelly_Heart_Pink:	return L"Prefab\\Jelly_Heart_Pink.pref";
	case EObjectID::Jelly_Heart_Yellow:	return L"Prefab\\Jelly_Heart_Yellow.pref";
	default:						return L"";
	}
}

CCookieSkillScript* GamePlayMgr::CreateSkillByCharacterType(ECharacterType _Type)
{
	if (m_PlayerObject == nullptr)
		return nullptr;

	switch (_Type)
	{
	case ECharacterType::TimeKeeper:
    {
		m_PlayerObject->AddComponent(new CTimeKeeperScript);

		auto pSkill = m_PlayerObject->GetScript<CTimeKeeperScript>();

		return (CCookieSkillScript*)pSkill.Get();
	}

	case ECharacterType::Lychee:
	{
		m_PlayerObject->AddComponent(new CLycheeScript);

		auto pSkill = m_PlayerObject->GetScript<CLycheeScript>();

		return (CCookieSkillScript*)pSkill.Get();
	}

	case ECharacterType::Default:
		break;
	default:
		break;
	}

	return nullptr;
}

Ptr<APrefab> GamePlayMgr::GetPrefabForCharType(ECharacterType _Type)
{
	switch (_Type)
	{
	case ECharacterType::TimeKeeper:	return FIND(APrefab, L"Prefab\\TimeKeeperCookie.pref");
	case ECharacterType::Lychee:		return FIND(APrefab, L"Prefab\\DragonCookie.pref");
	case ECharacterType::Default:		return FIND(APrefab, L"Prefab\\DefaultCookie.pref");
	default:						return nullptr;
	}
}
