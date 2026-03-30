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

		// Float params
		size_t floatCount = Info.FloatParams.size();
		fwrite(&floatCount, sizeof(size_t), 1, pFile);
		for (const auto& kv : Info.FloatParams)
		{
			size_t keyLen = kv.first.size();
			fwrite(&keyLen, sizeof(size_t), 1, pFile);
			fwrite(kv.first.c_str(), 1, keyLen, pFile);
			fwrite(&kv.second, sizeof(float), 1, pFile);
		}

		// Int params
		size_t intCount = Info.IntParams.size();
		fwrite(&intCount, sizeof(size_t), 1, pFile);
		for (const auto& kv : Info.IntParams)
		{
			size_t keyLen = kv.first.size();
			fwrite(&keyLen, sizeof(size_t), 1, pFile);
			fwrite(kv.first.c_str(), 1, keyLen, pFile);
			fwrite(&kv.second, sizeof(int), 1, pFile);
		}

		// Bool params (store as uint8_t)
		size_t boolCount = Info.BoolParams.size();
		fwrite(&boolCount, sizeof(size_t), 1, pFile);
		for (const auto& kv : Info.BoolParams)
		{
			size_t keyLen = kv.first.size();
			fwrite(&keyLen, sizeof(size_t), 1, pFile);
			fwrite(kv.first.c_str(), 1, keyLen, pFile);
			uint8_t v = kv.second ? 1 : 0;
			fwrite(&v, sizeof(uint8_t), 1, pFile);
		}
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

		// Float params
		size_t floatCount = 0;
		fread(&floatCount, sizeof(size_t), 1, pFile);
		for (size_t k = 0; k < floatCount; ++k)
		{
			size_t keyLen = 0;
			fread(&keyLen, sizeof(size_t), 1, pFile);
			std::string key(keyLen, '\0');
			fread(&key[0], 1, keyLen, pFile);
			float val = 0.f;
			fread(&val, sizeof(float), 1, pFile);
			m_vecSpawnInfo[i].FloatParams[key] = val;
		}

		// Int params
		size_t intCount = 0;
		fread(&intCount, sizeof(size_t), 1, pFile);
		for (size_t k = 0; k < intCount; ++k)
		{
			size_t keyLen = 0;
			fread(&keyLen, sizeof(size_t), 1, pFile);
			std::string key(keyLen, '\0');
			fread(&key[0], 1, keyLen, pFile);
			int val = 0;
			fread(&val, sizeof(int), 1, pFile);
			m_vecSpawnInfo[i].IntParams[key] = val;
		}

		// Bool params
		size_t boolCount = 0;
		fread(&boolCount, sizeof(size_t), 1, pFile);
		for (size_t k = 0; k < boolCount; ++k)
		{
			size_t keyLen = 0;
			fread(&keyLen, sizeof(size_t), 1, pFile);
			std::string key(keyLen, '\0');
			fread(&key[0], 1, keyLen, pFile);
			uint8_t v = 0;
			fread(&v, sizeof(uint8_t), 1, pFile);
			m_vecSpawnInfo[i].BoolParams[key] = (v != 0);
		}
	}

	fclose(pFile);
	return S_OK;
}
