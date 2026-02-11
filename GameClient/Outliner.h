#pragma once
#include "EditorUI.h"

/// Level 안에 어떤 오브젝트가 있는지 보여주는 UI
/// 거기서 오브젝트를 선택하면 Inspector 에서 그 오브젝트의 정보를 보여준다
class Outliner :
    public EditorUI
{
private:

public:
    virtual void Tick_UI() override;

public:
    Outliner();
	virtual ~Outliner();
};

