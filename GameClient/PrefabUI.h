#pragma once
#include "AssetUI.h"

class PrefabUI :
	public AssetUI
{
private:
	void ShowChildObject(class GameObject* _Child, int _Depth);

public:
	virtual void Tick_UI() override;

public:
	PrefabUI();
	virtual ~PrefabUI();
};

