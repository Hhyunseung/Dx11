#pragma once
#include "AssetUI.h"
class MaterialUI :
    public AssetUI
{
private:
    char        m_MaterialNameBuffer[256];
    Asset*      m_PrevMaterial;

public:
    virtual void Tick_UI() override;

private:
    void ShaderParameter();
    void SelectShader(DWORD_PTR _ListUI);

public:
    MaterialUI();
    virtual ~MaterialUI();
};

