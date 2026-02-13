#pragma once
#include "EditorUI.h"

#include "GameObject.h"
#include "ComponentUI.h"

/// 게임 오브젝트를 알려주면 게임 오브젝트의 정보를 알려주는 UI
class Inspector :
    public EditorUI
{
private:
    Ptr<GameObject>      m_TargetObject;
    Ptr<ComponentUI>     m_arrComUI[(UINT)COMPONENT_TYPE::END];

public:
    void SetTargetObject(Ptr<GameObject> _Object);

public:
    virtual void Tick_UI() override;

public:
    Inspector();
	virtual ~Inspector();
};

