#include "pch.h"
#include "AStageData.h"

AStageData::AStageData()
	: Asset(ASSET_TYPE::STAGE_DATA)
	, m_Row(0)
	, m_Col(0)
	, m_TileSize(0.f, 0.f)
	, m_Origin(0.f, 0.f)
{
}

AStageData::~AStageData()
{
}

void AStageData::Create(UINT _Row, UINT _Col, Vec2 _TileSize, Vec2 _Origin)
{
	m_Row = _Row;
	m_Col = _Col;
	m_TileSize = _TileSize;
	m_Origin = _Origin;
	m_Cells.assign(_Row * _Col, FTileContent{});
}

bool AStageData::CanPlaceObject(int row, int col) const
{
	if (!IsValidCell(row, col))
		return false;

	return GetCell(row, col).Type == EObjectType::None;
}

bool AStageData::PlaceObject(const FTileContent& content, int row, int col)
{
	if (!CanPlaceObject(row, col))
		return false;

	GetCell(row, col) = content;
	return true;
}

bool AStageData::RemoveObjectAt(int row, int col)
{
	if (!IsValidCell(row, col))
		return false;

	GetCell(row, col) = FTileContent{};
	return true;
}

int AStageData::Save(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");

	if (nullptr == pFile)
		return E_FAIL;

	// 그리드 정보 저장
	fwrite(&m_Row, sizeof(UINT), 1, pFile);
	fwrite(&m_Col, sizeof(UINT), 1, pFile);
	fwrite(&m_TileSize, sizeof(Vec2), 1, pFile);
	fwrite(&m_Origin, sizeof(Vec2), 1, pFile);

	// 셀 데이터 저장
	UINT CellCount = (UINT)m_Cells.size();
	fwrite(&CellCount, sizeof(UINT), 1, pFile);

	for (const auto& Cell : m_Cells)
	{
		fwrite(&Cell.Type, sizeof(EObjectType), 1, pFile);
		fwrite(&Cell.Data, sizeof(int), 1, pFile);
		fwrite(&Cell.VisualWidth, sizeof(int), 1, pFile);
		fwrite(&Cell.VisualHeight, sizeof(int), 1, pFile);
		fwrite(&Cell.SpawnOffset, sizeof(Vec2), 1, pFile);
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

	// 그리드 정보 로드
	fread(&m_Row, sizeof(UINT), 1, pFile);
	fread(&m_Col, sizeof(UINT), 1, pFile);
	fread(&m_TileSize, sizeof(Vec2), 1, pFile);
	fread(&m_Origin, sizeof(Vec2), 1, pFile);

	// 셀 데이터 로드
	UINT CellCount = 0;
	fread(&CellCount, sizeof(UINT), 1, pFile);

	m_Cells.resize(CellCount);

	for (UINT i = 0; i < CellCount; ++i)
	{
		fread(&m_Cells[i].Type, sizeof(EObjectType), 1, pFile);
		fread(&m_Cells[i].Data, sizeof(int), 1, pFile);
		fread(&m_Cells[i].VisualWidth, sizeof(int), 1, pFile);
		fread(&m_Cells[i].VisualHeight, sizeof(int), 1, pFile);
		fread(&m_Cells[i].SpawnOffset, sizeof(Vec2), 1, pFile);
	}

	fclose(pFile);
	return S_OK;
}
