#pragma once
#include "CRenderComponent.h"

#include "ATileMap.h"
#include "StructuredBuffer.h"

struct SpriteInfo
{
    Vec2 LeftTop;
    Vec2 Slice;
};


class CTileRender :
    public CRenderComponent
{
private:
	Ptr<ATileMap>           m_TileMap;
    vector<SpriteInfo>      m_vecSpriteInfo;
    Ptr<StructuredBuffer>   m_Buffer;

public:
    void SetTileMap(Ptr<ATileMap> _TileMap);

public:
    virtual void FinalTick() override;
    virtual void Render() override;
	virtual void CreateMaterial() override;

public:
    CTileRender();
    virtual ~CTileRender();

};

