#include "pch.h"
#include "ContentUI.h"

#include "AssetMgr.h"
#include "TreeUI.h"
#include "Asset.h"

ContentUI::ContentUI()
	 : EditorUI("ContentUI")
{
	m_Tree = new TreeUI;
	m_Tree->SetSeperator(false);
	AddChildUI(m_Tree.Get());

	// Asset 내용을 트리에 반영
	Renew();
}

ContentUI::~ContentUI()
{
}


void ContentUI::Renew()
{
	// 트리 비우기
	m_Tree->Clear();

	// 에셋 종류별로 Tree 에 추가하기
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		// 에셋의 이름에 해당하는 노드를 추가 (enum 타입을 문자열로 바꿔서 추가)
		Ptr<TreeNode> pNode = m_Tree->AddItem(nullptr, ToString((ASSET_TYPE)i));
		pNode->SetFramed(true);

		// 해당 에셋 모든 이름을 받아와서 하위 자식으로 추가
		vector<wstring> vecAssetNames;
		AssetMgr::GetInst()->GetAssetNames((ASSET_TYPE)i, vecAssetNames);


		for (const auto& Name : vecAssetNames)
		{
			Ptr<Asset> pAsset = AssetMgr::GetInst()->FindAsset(Name, (ASSET_TYPE)i);
			m_Tree->AddItem(pNode, string(Name.begin(), Name.end()), (DWORD_PTR)pAsset.Get());
		}
	}
}

void ContentUI::AddAsset(Ptr<TreeNode> _ParentNode, Ptr<Asset> _Asset)
{

}

void ContentUI::Tick_UI()
{
}

