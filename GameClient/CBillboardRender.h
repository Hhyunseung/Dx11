#pragma once
#include "CRenderComponent.h"

class CBillboardRender :
    public CRenderComponent
{
private:
    Vec2    m_BillboardScale;

public:
	GET_SET(Vec2, BillboardScale);

public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void CreateMaterial() override;

public:
    CBillboardRender();
    virtual ~CBillboardRender();

};

