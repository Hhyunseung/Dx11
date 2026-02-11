#pragma once
#include "EditorUI.h"

/// 게임 오브젝트를 알려주면 게임 오브젝트의 정보를 알려주는 UI
class Inspector :
    public EditorUI
{
private:

public:
    virtual void Tick_UI() override;

public:
    Inspector();
	virtual ~Inspector();
};

