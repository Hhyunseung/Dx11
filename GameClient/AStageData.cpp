#include "pch.h"
#include "AStageData.h"

AStageData::AStageData()
	: Asset(ASSET_TYPE::STAGE_DATA)
{
}

AStageData::~AStageData()
{
}


int AStageData::Save(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");

	if (nullptr == pFile)
		return E_FAIL;

	// SpawnInfo 개수 저장
	UINT SpawnInfoCount = (UINT)m_vecSpawnInfo.size();
	fwrite(&SpawnInfoCount, sizeof(UINT), 1, pFile);

	// 각 SpawnInfo 저장
	for (const auto& Info : m_vecSpawnInfo)
	{
		fwrite(&Info.ObjectID, sizeof(int), 1, pFile);
		fwrite(&Info.LayerIdx, sizeof(int), 1, pFile);
		fwrite(&Info.WorldPos, sizeof(Vec2), 1, pFile);
		fwrite(&Info.Scale, sizeof(Vec2), 1, pFile);
	}

	fclose(pFile);
	return S_OK;
}

int AStageData::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	if (nullptr == pFile)
		return E_FAIL;

	// 기존 데이터 초기화
	m_vecSpawnInfo.clear();

	// SpawnInfo 개수 읽기
	UINT SpawnInfoCount = 0;
	fread(&SpawnInfoCount, sizeof(UINT), 1, pFile);

	// 벡터 크기 할당 후 읽기
	m_vecSpawnInfo.resize(SpawnInfoCount);

	for (UINT i = 0; i < SpawnInfoCount; ++i)
	{
		fread(&m_vecSpawnInfo[i].ObjectID, sizeof(int), 1, pFile);
		fread(&m_vecSpawnInfo[i].LayerIdx, sizeof(int), 1, pFile);
		fread(&m_vecSpawnInfo[i].WorldPos, sizeof(Vec2), 1, pFile);
		fread(&m_vecSpawnInfo[i].Scale, sizeof(Vec2), 1, pFile);
	}

	fclose(pFile);
	return S_OK;
}
