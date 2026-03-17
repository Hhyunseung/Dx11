#pragma once
#include "EditorUI.h"

#include "GameObject.h"
#include "ComponentUI.h"
#include "ScriptUI.h"
#include "AssetUI.h"

/// 게임 오브젝트를 알려주면 게임 오브젝트의 정보를 알려주는 UI
class Inspector :
	public EditorUI
{
private:
	Ptr<GameObject>      m_TargetObject;
	Ptr<ComponentUI>     m_arrComUI[(UINT)COMPONENT_TYPE::END];
	vector<Ptr<ScriptUI>> m_vecScriptUI;

	Ptr<Asset>           m_TargetAsset;
	Ptr<AssetUI> 	     m_arrAssetUI[(UINT)ASSET_TYPE::END];

	char				 m_PrefabNameBuffer[256];
	char				 m_ObjectNameBuffer[256];
	GameObject*			 m_PrevTargetObject;

public:
	Ptr<GameObject> GetTargetObejct() { return m_TargetObject; }
	void SetTargetObject(Ptr<GameObject> _Object);

	Ptr<Asset> GetTargetAsset() { return m_TargetAsset; }
	void SetTargetAsset(Ptr<Asset> _Asset);

	void SavePrefab();
	void DeleteTargetObject();


public:
	virtual void Tick_UI() override;

private:
	void CreateChildUI();

public:
	Inspector();
	virtual ~Inspector();
};

