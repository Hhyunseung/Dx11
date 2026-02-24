#include "pch.h"
#include "TreeUI.h"


TreeNode::TreeNode()
	: Parent(nullptr)
{
	// 각 노드가 표기하려는 이름이 같을 수가 있기 때문에,
	// 보여주려는 이름 뒤에 붙을 고유 문자열을 미리 생성해둔다
	char szKey[50] = {};
	sprintf_s(szKey, 50, "##TreeNode%d", GetID());
	Key = szKey;
}

TreeNode::~TreeNode()
{
}

void TreeNode::Tick()
{
	/// 게임오브젝트 이름을 들고있는 노드를 TreeNode 위젯으로 출력

	// TreeNode Flag 설정
	UINT Flags = ImGuiTreeNodeFlags_SpanFullWidth;

	// 노드가 자식노드를 보유하고 있지 않으면 Leaf 플래그 추가
	if (vecChildNode.empty())
		Flags |= ImGuiTreeNodeFlags_Leaf;

	string NodeName = Str + Key;

	// 트리 노드에 등록된 문자열을 Key 로 해서 출력
	if (ImGui::TreeNodeEx(NodeName.c_str(), Flags))
	{
		for (size_t i = 0; i < vecChildNode.size(); ++i)
		{
			vecChildNode[i]->Tick();
		}

		ImGui::TreePop();
	}
}








TreeUI::TreeUI()
	: EditorUI("TreeUI")
{
}

TreeUI::~TreeUI()
{
}

void TreeUI::Tick_UI()
{
	for (size_t i = 0; i < m_vecNode.size(); ++i)
	{
		m_vecNode[i]->Tick();
	}
}

Ptr<TreeNode> TreeUI::AddItem(Ptr<TreeNode> _ParentNode, string _String)
{
	Ptr<TreeNode> pNewNode = new TreeNode;
	pNewNode->Str = _String;

	// 최상위 부모노드로 추가
	if (nullptr == _ParentNode)
	{
		m_vecNode.push_back(pNewNode);
	}

	// 특정 노드 밑에 자식으로 추가
	else
	{
		_ParentNode->AddChildNode(pNewNode);
	}

	return pNewNode;
}
