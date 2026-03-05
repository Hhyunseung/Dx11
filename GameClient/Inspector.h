#pragma once
#include "EditorUI.h"

#include "GameObject.h"
#include "ComponentUI.h"
#include "AssetUI.h"

/// 게임 오브젝트를 알려주면 게임 오브젝트의 정보를 알려주는 UI
class Inspector :
    public EditorUI
{
private:
    Ptr<GameObject>      m_TargetObject;
    Ptr<ComponentUI>     m_arrComUI[(UINT)COMPONENT_TYPE::END];

    Ptr<Asset>           m_TargetAsset;
	Ptr<AssetUI> 	     m_arrAssetUI[(UINT)ASSET_TYPE::END];

public:
    Ptr<GameObject> GetTargetObejct() { return m_TargetObject; }
    void SetTargetObject(Ptr<GameObject> _Object);

    Ptr<Asset> GetTargetAsset() { return m_TargetAsset; }
    void SetTargetAsset(Ptr<Asset> _Asset);

public:
    virtual void Tick_UI() override;

private:
    void CreateChildUI();

public:
    Inspector();
	virtual ~Inspector();
};

