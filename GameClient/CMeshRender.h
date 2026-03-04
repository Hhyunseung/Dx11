#pragma once
#include "CRenderComponent.h"

#include "AMesh.h"
#include "AGraphicShader.h"
#include "ATexture.h"
#include "AMaterial.h"

class CMeshRender :
    public CRenderComponent
{
private:

public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void CreateMaterial() override {}

    CLONE(CMeshRender);

public:
    CMeshRender();
	virtual ~CMeshRender();
};

