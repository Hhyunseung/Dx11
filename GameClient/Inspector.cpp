#include "pch.h"
#include "Inspector.h"

#include "LevelMgr.h"

#include "TransformUI.h"
#include "Collider2DUI.h"
#include "CameraUI.h"
#include "Light2DUI.h"

Inspector::Inspector()
	: EditorUI("Inspector")
{
	CreateChildUI();

	SetTargetObject(nullptr);
}

Inspector::~Inspector()
{

}

void Inspector::SetTargetObject(Ptr<GameObject> _Object)
{
	// 입력된 게임 오브젝트의 정보를 보여줄 ComponentUI 들을 활성화 시킨다
	m_TargetObject = _Object;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComUI[i] == nullptr)
			continue;

		m_arrComUI[i]->SetTarget(m_TargetObject);
	}

	if (nullptr != m_TargetObject)
	{
		// 오브젝트의 Script 에 대응하는 ScriptUI 를 활성화 / 비활성화 시킨다
		const vector<Ptr<CScript>>& vecScripts = m_TargetObject->GetScripts();

		// 오브젝트가 보유한 Script 개수에 비해서 대응할 ScriptUI 의 개수가 모자라면 추가한다
		if (m_vecScriptUI.size() < vecScripts.size())
		{
			int AddCount = vecScripts.size() - m_vecScriptUI.size();

			for (int i = 0; i < AddCount; ++i)
			{
				ScriptUI* pScriptUI = new ScriptUI;
				pScriptUI->SetSizeAsChild(Vec2(0.f, 150.f));
				AddChildUI(pScriptUI);

				m_vecScriptUI.push_back(pScriptUI);
			}
		}

		// 오브젝트에서 가져온 Script 를 각각의 ScriptUI 에게 세팅해준다
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			if (vecScripts.size() <= i)
				m_vecScriptUI[i]->SetScript(nullptr);
			else
				m_vecScriptUI[i]->SetScript(vecScripts[i].Get());
		}
	}

	else
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetScript(nullptr);
		}
	}

	// AssetUI 를 비활성화한다.
	m_TargetAsset = nullptr;
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		if (nullptr != m_arrAssetUI[i])
			m_arrAssetUI[i]->SetActive(false);
	}
}

void Inspector::SetTargetAsset(Ptr<Asset> _Asset)
{
	// ComponentUI 들을 비활성화 시킨다
	SetTargetObject(nullptr);
	
	m_TargetAsset = _Asset;
	if (nullptr == m_TargetAsset)
	{
		for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
			m_arrAssetUI[i]->SetActive(false);
	}

	else
	{
		ASSET_TYPE Type = m_TargetAsset->GetType();
		m_arrAssetUI[(UINT)Type]->SetActive(true);
		m_arrAssetUI[(UINT)Type]->SetTargetAsset(m_TargetAsset);
	}
}

void Inspector::Tick_UI()
{
	if (m_TargetObject == nullptr)
		return;

	wstring Name = m_TargetObject->GetName();
	string strName(string(Name.begin(), Name.end()));

	if (strName.empty())
		strName = "No Name";

	ImGui::Button(strName.c_str());

	ImGui::Separator();
}
