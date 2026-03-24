#include "pch.h"
#include "GamePlayMgr.h"

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
	case EObjectID::DefaultJelly:	return L"Prefab\\DefaultJelly.pref";
	case EObjectID::Coin1:			return L"Prefab\\Coin1.pref";
	case EObjectID::Coin2:			return L"Prefab\\Coin2.pref";
	case EObjectID::BearYellow:		return L"Prefab\\BearJelly_Yellow.pref";
	case EObjectID::BearPink:		return L"Prefab\\BearJelly_Pink.pref";
	case EObjectID::BearBlue:		return L"Prefab\\BearJelly_Blue.pref";
	case EObjectID::BearBigYellow:	return L"Prefab\\Jelly_Bearbig_z65.pref";
	case EObjectID::BearRainbow:	return L"Prefab\\BearJelly_Rainbow.pref";
	case EObjectID::SpecialBonus_1:	return L"Prefab\\SpecialBonus.pref";
	case EObjectID::Item:			return L"Prefab\\Item.pref";
	default:						return L"";
	}
}
