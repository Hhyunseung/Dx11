#pragma once
#include "AssetUI.h"

class PrefabUI :
	public AssetUI
{
private:
	int m_TargetLayer;
	GameObject* m_TempInstantiatedObj;

	void ShowChildObject(class GameObject* _Child, int _Depth);

public:
	virtual void Tick_UI() override;

public:
	PrefabUI();
	virtual ~PrefabUI();
};

