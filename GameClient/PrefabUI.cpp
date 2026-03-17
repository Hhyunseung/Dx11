#include "pch.h"
#include "PrefabUI.h"

#include "Component.h"
#include "PathMgr.h"
#include "AssetMgr.h"
#include "LevelMgr.h"


PrefabUI::PrefabUI()
	: AssetUI(ASSET_TYPE::PREFAB)
{

}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::Tick_UI()
{
	OutputTitle();

	Ptr<APrefab> pPrefab = (APrefab*)GetTargetAsset().Get();
	if (nullptr == pPrefab)
		return;

	// Prefab 이름 표시
	string Key = string(pPrefab->GetKey().begin(), pPrefab->GetKey().end());
	ImGui::Text("Prefab Name");
	ImGui::SameLine(150);
	ImGui::TextColored(ImVec4(0, 1, 1, 1), "%s", Key.c_str());

	ImGui::Spacing();

	// Prefab 파일 경로 표시
	wstring RelativePath = pPrefab->GetRelativePath();
	string PathStr = string(RelativePath.begin(), RelativePath.end());
	ImGui::Text("File Path");
	ImGui::SameLine(150);
	ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1), "%s", PathStr.c_str());

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// Prefab의 GameObject 정보 표시
	Ptr<GameObject> pProtoObj = pPrefab->m_ProtoObj;
	if (nullptr != pProtoObj)
	{
		ImGui::Text("Prototype Object Info");
		ImGui::Separator();

		// GameObject 이름
		string ObjName = string(pProtoObj->GetName().begin(), pProtoObj->GetName().end());
		ImGui::Text("Object Name");
		ImGui::SameLine(150);
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "%s", ObjName.c_str());

		ImGui::Spacing();

		// 컴포넌트 목록 표시
		ImGui::Text("Components");
		ImGui::Indent(20.f);

		//for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		//{
		//	CComponent* pCom = pProtoObj->GetComponent((COMPONENT_TYPE)i);
		//	if (nullptr != pCom)
		//	{
		//		const char* ComName = ToString((COMPONENT_TYPE)i);
		//		ImGui::BulletText("%s", ComName);
		//	}
		//}

		// Script 목록
		const vector<Ptr<CScript>>& vecScripts = pProtoObj->GetScripts();
		if (!vecScripts.empty())
		{
			for (size_t i = 0; i < vecScripts.size(); ++i)
			{
				string ScriptName = string(vecScripts[i]->GetName().begin(), vecScripts[i]->GetName().end());
				ImGui::BulletText("Script: %s", ScriptName.c_str());
			}
		}

		ImGui::Unindent(20.f);
		ImGui::Spacing();

		// 자식 오브젝트 개수 및 트리 표시
		const vector<Ptr<GameObject>>& vecChild = pProtoObj->GetChild();
		ImGui::Text("Child Objects: %d", (int)vecChild.size());

		if (!vecChild.empty())
		{
			ImGui::Indent(20.f);
			for (size_t i = 0; i < vecChild.size(); ++i)
			{
				ShowChildObject(vecChild[i].Get(), 0);
			}
			ImGui::Unindent(20.f);
		}
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Instantiate 버튼
		if (ImGui::Button("Instantiate to Current Level", ImVec2(200, 30)))
		{
			Ptr<ALevel> pCurLevel = LevelMgr::GetInst()->GetCurrentLevel();
			if (nullptr != pCurLevel)
			{
				GameObject* pNewObj = pPrefab->Instantiate();
				if (nullptr != pNewObj)
				{
					// 기본적으로 0번 레이어에 추가 (원하는 레이어로 변경 가능)
					pCurLevel->AddObject(0, pNewObj);
					ImGui::OpenPopup("Instantiate Success");
				}
				else
				{
					ImGui::OpenPopup("Error##InstantiateFailed");
				}
			}
		}

		// Instantiate 성공 팝업
		if (ImGui::BeginPopupModal("Instantiate Success", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Prefab instantiated successfully!");
			ImGui::Text("Added to Layer 0 of current level");
			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		// Instantiate 실패 팝업
		if (ImGui::BeginPopupModal("Error##InstantiateFailed", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Failed to instantiate Prefab!");
			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	else
	{
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "Prefab has no prototype object!");
	}
}

void PrefabUI::ShowChildObject(GameObject* _Child, int _Depth)
{
	if (nullptr == _Child)
		return;

	// 들여쓰기 (깊이에 따라)
	for (int i = 0; i < _Depth; ++i)
	{
		ImGui::Indent(15.f);
	}

	// 자식 오브젝트 이름 표시 (TreeNode 사용)
	string ChildName = string(_Child->GetName().begin(), _Child->GetName().end());
	bool bOpen = ImGui::TreeNode(ChildName.c_str());

	if (bOpen)
	{
		//// 컴포넌트 목록 표시
		//ImGui::Indent(10.f);
		//for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		//{
		//	CComponent* pCom = _Child->GetComponent((COMPONENT_TYPE)i);
		//	if (nullptr != pCom)
		//	{
		//		const char* ComName = ToString((COMPONENT_TYPE)i);
		//		ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1), "- %s", ComName);
		//	}
		//}

		// Script 목록
		const vector<Ptr<CScript>>& vecScripts = _Child->GetScripts();
		for (size_t i = 0; i < vecScripts.size(); ++i)
		{
			string ScriptName = string(vecScripts[i]->GetName().begin(), vecScripts[i]->GetName().end());
			ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1), "- Script: %s", ScriptName.c_str());
		}
		ImGui::Unindent(10.f);

		// 재귀적으로 자식의 자식들 표시
		const vector<Ptr<GameObject>>& vecGrandChild = _Child->GetChild();
		for (size_t i = 0; i < vecGrandChild.size(); ++i)
		{
			ShowChildObject(vecGrandChild[i].Get(), 0); // TreeNode가 들여쓰기를 처리하므로 depth는 0으로
		}

		ImGui::TreePop();
	}

	// 들여쓰기 복원
	for (int i = 0; i < _Depth; ++i)
	{
		ImGui::Unindent(15.f);
	}
}