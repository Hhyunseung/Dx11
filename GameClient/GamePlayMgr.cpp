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
}

void GamePlayMgr::Clear()
{
	m_SeletectedCharacterPrefab = nullptr;
	m_PlayerObject = nullptr;
	m_Score = 0;
}
