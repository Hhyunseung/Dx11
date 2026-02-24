#include "pch.h"
#include "Outliner.h"

#include "TreeUI.h"

#include "LevelMgr.h"
#include "ALevel.h"

Outliner::Outliner()
	: EditorUI("Outliner")
{
	m_Tree = new TreeUI;
	AddChildUI(m_Tree.Get());

	Renew();
}

Outliner::~Outliner()
{
}


void Outliner::Renew()
{
	// 트리에 표기된 오브젝트 정보를 전부 삭제
	m_Tree->Clear();

	// 현재 레벨 가져옴
	Ptr<ALevel> pLevel = LevelMgr::GetInst()->GetCurrentLevel();
	if (pLevel == nullptr)
		return;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 각 레이어에 등록된 최상위 부모 오브젝트를 가져옴
		const vector<Ptr<GameObject>>& vecParents = pLevel->GetLayer(i)->GetParentObjects();

		// 최상위 부모 오브젝트들을 트리에 추가
		for (const auto& Object : vecParents)
		{
			AddGameObject(nullptr, Object);
		}
	}
}

void Outliner::AddGameObject(Ptr<TreeNode> _ParentNode, Ptr<GameObject> _Object)
{
	string ObjectName = string(_Object->GetName().begin(), _Object->GetName().end());

	if (ObjectName.empty())
		ObjectName = "No Name";

	Ptr<TreeNode> pNewNode = m_Tree->AddItem(_ParentNode, ObjectName.c_str());

	for (size_t i = 0; i < _Object->GetChild().size(); ++i)
	{
		AddGameObject(pNewNode, _Object->GetChild()[i]);
	}
}


void Outliner::Tick_UI()
{

}
