#pragma once
#include "Asset.h"

class AStageData :
    public Asset
{
private:
	UINT			m_Row;
	UINT			m_Col;
	Vec2			m_TileSize;			// 타일맵을 구성하는 타일 1개의 크기
	Vec2			m_Origin;			// 타일맵의 원점 위치 (월드 좌표 기준)

	vector<FTileContent> m_Cells;

public:
	int GetRow() const { return m_Row; }
	int GetCol() const { return m_Col; }
	Vec2 GetTileSize() const { return m_TileSize; }
	Vec2 GetOrigin() const { return m_Origin; }

	void SetOrigin(Vec2 _Origin) { m_Origin = _Origin; }
	void SetTileSize(Vec2 _TileSize) { m_TileSize = _TileSize; }

	int GetIndex(int _Row, int _Col) const { return _Row * m_Col + _Col; }
	bool IsValidCell(int _Row, int _Col) const { return _Row >= 0 && _Row < (int)m_Row && _Col >= 0 && _Col < (int)m_Col; }

	// 그리드 좌표를 월드 좌표(셀 중심)로 변환
	Vec2 GridToWorldCenter(int _Row, int _Col) const
	{
		float x = m_Origin.x + _Col * m_TileSize.x + m_TileSize.x * 0.5f;
		float y = m_Origin.y + _Row * m_TileSize.y + m_TileSize.y * 0.5f;
		return Vec2(x, y);
	}

	FTileContent& GetCell(int _Row, int _Col) { assert(IsValidCell(_Row, _Col)); return m_Cells[GetIndex(_Row, _Col)]; }
	const FTileContent& GetCell(int _Row, int _Col) const { assert(IsValidCell(_Row, _Col)); return m_Cells[GetIndex(_Row, _Col)]; }

public:
	void Create(UINT _Row, UINT _Col, Vec2 _TileSize, Vec2 _Origin);
	
	bool CanPlaceObject(int row, int col) const;
	bool PlaceObject(const FTileContent& content, int row, int col);
	bool RemoveObjectAt(int row, int col);

public:
	virtual int Save(const wstring& _FilePath);
	virtual int Load(const wstring& _FilePath);
	CLONE(AStageData);

public:
	AStageData();
	virtual ~AStageData();
};

