#include "pch.h"
#include "GamePlayMgr.h"
#include "ObjectPoolMgr.h"

#include "..\GameClient\Source\Scripts\CPlayerScript.h"
#include "..\GameClient\Source\Scripts\CCookieSkillScript.h"
#include "..\GameClient\Source\Scripts\CTimeKeeperScript.h"

GamePlayMgr::GamePlayMgr()
	: m_PlayerObject(nullptr)
	, m_Score(0)
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
	m_ScrollSpeed = 300.f;	// 기본 스크롤 속도

	// 기본 StageData 생성
	if (nullptr == m_StageData)
	{
		CreateNewStageData();
	}

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
