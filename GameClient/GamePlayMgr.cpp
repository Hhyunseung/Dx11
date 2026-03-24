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
	//m_StageData->SetKey(L"NewStageData");
}
