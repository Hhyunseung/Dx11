#include "pch.h"
#include "GamePlayMgr.h"
#include "ObjectPoolMgr.h"
#include "AssetMgr.h"

#include "..\GameClient\Source\Scripts\CPlayerScript.h"
#include "..\GameClient\Source\Scripts\CCookieSkillScript.h"
#include "..\GameClient\Source\Scripts\CTimeKeeperScript.h"

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
	//Ptr<APrefab> pPrefab = FIND(APrefab, L"Prefab\\TimeKeeperCookie.pref");
	Ptr<APrefab> pPrefab = FIND(APrefab, L"Prefab\\DragonCookie.pref");
	m_SeletectedCharacterPrefab = pPrefab; // 선택된 캐릭터 프리팹 저장
	m_SelectedCharacterType = ECharacterType::TimeKeeper; // 기본 캐릭터 타입 설정

	Ptr<APrefab> pPrefabUI = FIND(APrefab, L"Prefab\\GamePlayUIRoot.pref");
	m_GamePlayUIRootPrefab = pPrefabUI; // UI 루트 프리팹 저장

	Ptr<AStageData> pStageData = LOAD(AStageData, L"StageData\\TestStage.stage");
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
	case EObjectID::Coin1:			return L"Prefab\\Jelly_CoinGold.pref";
	case EObjectID::Coin2:			return L"Prefab\\Jelly_CoinSilver.pref";
	case EObjectID::BearYellow:		return L"Prefab\\Jelly_Bearyellow.pref";
	case EObjectID::BearPink:		return L"Prefab\\Jelly_Bearpink.pref";
	case EObjectID::BearBlue:		return L"Prefab\\Jelly_Bearice.pref";
	case EObjectID::BearBigYellow:	return L"Prefab\\Jelly_Bearbig.pref";
	case EObjectID::BearRainbow:	return L"Prefab\\Jelly_BearRainbow.pref";
	case EObjectID::SpecialBonus_1:	return L"Prefab\\Jelly_SpecialBonus_1.pref";
	case EObjectID::SpecialBonus_2:	return L"Prefab\\Jelly_SpecialBonus_2.pref";
	case EObjectID::Item:			return L"Prefab\\Item.pref";
	case EObjectID::Obstacle_bl1:	return L"Prefab\\Obstacle_bl1.pref";
	case EObjectID::Obstacle_cb2:	return L"Prefab\\Obstacle_cb2.pref";
	case EObjectID::Obstacle_tm001:	return L"Prefab\\Obstacle_tm001.pref";
	case EObjectID::Obstacle_jp1down:	return L"Prefab\\Obstacle_jp1down.pref";
	case EObjectID::Obstacle_tm001_sdswing:	return L"Prefab\\Obstacle_tm001_sdswing.pref";
	case EObjectID::TileShipeStage_1_fh_m:	return L"Prefab\\TileShipeStage_1_fh_m.pref";
	case EObjectID::TileShipeStage_1_tb_s:	return L"Prefab\\TileShipeStage_1_tb_s.pref";
	case EObjectID::Item_Giant:		return L"Prefab\\Item_Giant.pref";
	case EObjectID::Item_Boost:	return L"Prefab\\Item_Boost.pref";
	case EObjectID::Item_Magnet:		return L"Prefab\\Item_Magnet.pref";
	case EObjectID::Item_HealA:		return L"Prefab\\Item_HealA.pref";
	case EObjectID::Item_HealB:		return L"Prefab\\Item_HealB.pref";
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

		// case ECharacterType::Knight:
		//     return _PlayerObject->AddComponent(new CKnightSkillScript);

		// case ECharacterType::Wizard:
		//     return _PlayerObject->AddComponent(new CWizardSkillScript);

	default:
		break;
	}

	return nullptr;
}
