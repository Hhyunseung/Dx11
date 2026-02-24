#pragma once
#include "EditorUI.h"

struct TreeNode
    : public Entity
{
public:
    string                Str;
	string 			      Key;
    TreeNode*             Parent;
    vector<Ptr<TreeNode>> vecChildNode;

public:
    void AddChildNode(Ptr<TreeNode> _Node)
    {
        vecChildNode.push_back(_Node);
        _Node->Parent = this;
    }

public:
    void Tick();

public:
    TreeNode();
    virtual ~TreeNode();
};





class TreeUI :
    public EditorUI
{
private:
    vector<Ptr<TreeNode>> m_vecNode;

public:
    virtual void Tick_UI() override;

public:
	void Clear() { m_vecNode.clear(); }
	///  자식 노드를 추가하는 함수 /// ParentNode 가 nullptr 이면, 최상위 노드로 추가
    Ptr<TreeNode> AddItem(Ptr<TreeNode> _ParentNode, string _String);

public:
    TreeUI();
    virtual ~TreeUI();
};

