#include "pch.h"
#include "GameDataMgr.h"

#include <fstream>

GameDataMgr::GameDataMgr()
	: m_TotalCoin(0)
	, m_BestScore(0)
{
}

GameDataMgr::~GameDataMgr()
{
}

void GameDataMgr::Init()
{
	m_TotalCoin = 0;
	m_BestScore = 0;
	m_SelectedCharacter.clear();
	m_SelectedStage.clear();

	Load();
}

void GameDataMgr::Clear()
{
	m_TotalCoin = 0;
	m_BestScore = 0;
	m_SelectedCharacter.clear();
	m_SelectedStage.clear();
}

void GameDataMgr::AddTotalCoin(int _Amount)
{
	if (_Amount <= 0)
		return;

	m_TotalCoin += _Amount;
}

bool GameDataMgr::UseCoin(int _Amount)
{
	if (_Amount <= 0)
		return false;

	if (m_TotalCoin < _Amount)
		return false;

	m_TotalCoin -= _Amount;
	return true;
}

void GameDataMgr::UpdateBestScore(int _Score)
{
	if (_Score > m_BestScore)
	{
		m_BestScore = _Score;
	}
}

bool GameDataMgr::Save()
{
	std::wofstream fout(L"SaveData.txt");
	if (!fout.is_open())
		return false;

	fout << m_TotalCoin << L"\n";
	fout << m_BestScore << L"\n";
	fout << m_SelectedCharacter << L"\n";
	fout << m_SelectedStage << L"\n";

	fout.close();
	return true;
}

bool GameDataMgr::Load()
{
	std::wifstream fin(L"SaveData.txt");
	if (!fin.is_open())
		return false;

	fin >> m_TotalCoin;
	fin.ignore();

	fin >> m_BestScore;
	fin.ignore();

	std::getline(fin, m_SelectedCharacter);
	std::getline(fin, m_SelectedStage);

	fin.close();
	return true;
}