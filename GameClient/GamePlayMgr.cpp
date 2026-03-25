#include "pch.h"
#include "GamePlayMgr.h"
#include "ObjectPoolMgr.h"

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
	m_PlayerObject = nullptr;
	m_Score = 0;

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
	m_SeletectedCharacterPrefab = nullptr;
	m_PlayerObject = nullptr;
	m_Score = 0;
}

void GamePlayMgr::CreateNewStageData()
{
	m_StageData = new AStageData;
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
	default:						return L"";
	}
}
