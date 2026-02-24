#pragma once
#include "EditorUI.h"
#include "TreeUI.h"

#include "GameObject.h"

/// Level 안에 어떤 오브젝트가 있는지 보여주는 UI
/// 거기서 오브젝트를 선택하면 Inspector 에서 그 오브젝트의 정보를 보여준다
class Outliner :
    public EditorUI
{
private:
    Ptr<TreeUI>     m_Tree;

public:
    virtual void Tick_UI() override;

    // Tree 갱신, 현재 레벨의 최신 상태를 Tree 에 표시
    void Renew();

private:
    void AddGameObject(Ptr<TreeNode> _ParentNode, Ptr<GameObject> _Object);

public:
    Outliner();
	virtual ~Outliner();
};

