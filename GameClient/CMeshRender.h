#pragma once
#include "CRenderComponent.h"

#include "AMesh.h"
#include "AGraphicShader.h"
#include "ATexture.h"
#include "AMaterial.h"

class CMeshRender :
    public CRenderComponent
{
public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void CreateMaterial() override {}

public:
    CMeshRender();
	virtual ~CMeshRender();
};

