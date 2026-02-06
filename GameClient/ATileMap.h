#pragma once
#include "Asset.h"

#include "ATexture.h"
#include "ASprite.h"

class ATileMap :
    public Asset
{
private:
	UINT					m_Row;				// 타일맵의 행 개수
	UINT 					m_Col;				// 타일맵의 열 개수
	Vec2					m_TileSize;			// 타일맵을 구성하는 타일 1개의 크기
	
///  같은 Atlas 를 사용하도록 설계하기 위해 Atlas 멤버변수 추가
	Ptr<ATexture>			m_Atlas;			// 타일맵을 구성하는 Sprite 들의 공통 아틀라스 텍스쳐
	vector<Ptr<ASprite>>	m_vecSpriteInfo;	// 

public:
	void SetRowCol(UINT _Row, UINT _Col);
	void SetSprite(UINT _Row, UINT _Col, Ptr<ASprite> _Sprite);

	UINT GetRow() { return m_Row; }
	UINT GetCol() { return m_Col; }

	const vector<Ptr<ASprite>>& GetSprites() { return m_vecSpriteInfo; }
	
	GET_SET(Vec2, TileSize);
	GET_SET(Ptr<ATexture>, Atlas);
public:
    ATileMap();
	virtual ~ATileMap();
};

