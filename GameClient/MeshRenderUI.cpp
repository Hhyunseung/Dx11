#include "pch.h"
#include "MeshRenderUI.h"

#include "AssetMgr.h"
#include "ListUI.h"
#include "EditorMgr.h"

MeshRenderUI::MeshRenderUI()
	: ComponentUI(COMPONENT_TYPE::MESHRENDER, "MeshRenderUI")
{
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::Tick_UI()
{
	OutputTitle("MeshRender");

	Ptr<CMeshRender> pMeshRender = GetTarget()->MeshRender();

	// ====
	// Mesh 
	// ====
	ImGui::Text("Mesh");
	ImGui::SameLine(120);

	Ptr<AMesh> pMesh = pMeshRender->GetMesh();
	string MeshKey;

	if (pMesh == nullptr)
		MeshKey = "None";
	else
		MeshKey = string(pMesh->GetKey().begin(), pMesh->GetKey().end());

	ImGui::InputText("##MeshName", MeshKey.data(), MeshKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", Vec2(18.f, 18.f)))
	{
		// 버튼이 눌리면, 리스트UI 를 찾아서 활성화 시키고, 출력시키고 싶은 문자열을 ListUI 에 등록시킨다
		Ptr<ListUI> pUI = dynamic_cast<ListUI*>(EditorMgr::GetInst()->FindUI("ListUI").Get());
		assert(pUI.Get());

		pUI->SetUIName("Mesh List");

		vector<wstring> vecMeshNames;
		AssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MESH, vecMeshNames);
		pUI->AddString(vecMeshNames);
		pUI->AddDelegate(this, (DELEGATE_1)&MeshRenderUI::SelectMesh);
		pUI->SetActive(true);
	}
	

	// ========
	// Material
	// ========
	ImGui::Text("Material");	
	ImGui::SameLine(120);

	Ptr<AMaterial> pMtrl = pMeshRender->GetMaterial();
	string MtrlKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	ImGui::InputText("##MtrlName", MtrlKey.data(), MtrlKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();
	if (ImGui::Button("##MtrlBtn", Vec2(18.f, 18.f)))
	{
		// 버튼이 눌리면, 리스트UI 를 찾아서 활성화 시키고, 출력시키고 싶은 문자열을 ListUI 에 등록시킨다
		Ptr<ListUI> pUI = dynamic_cast<ListUI*>(EditorMgr::GetInst()->FindUI("ListUI").Get());
		assert(pUI.Get());

		pUI->SetUIName("Material List");

		vector<wstring> vecMtrlNames;
		AssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MATERIAL, vecMtrlNames);
		pUI->AddString(vecMtrlNames);
		pUI->AddDelegate(this, (DELEGATE_1)&MeshRenderUI::SelectMtrl);
		pUI->SetActive(true);
	}
}

void MeshRenderUI::SelectMesh(DWORD_PTR _ListUI)
{
	Ptr<ListUI> pListUI = ((ListUI*)_ListUI);

	wstring key = wstring(pListUI->GetSelectedString().begin(), pListUI->GetSelectedString().end());

	Ptr<AMesh> pMesh = FIND(AMesh, key);

	GetTarget()->MeshRender()->SetMesh(pMesh);
}

void MeshRenderUI::SelectMtrl(DWORD_PTR _ListUI)
{
	Ptr<ListUI> pListUI = ((ListUI*)_ListUI);

	wstring key = wstring(pListUI->GetSelectedString().begin(), pListUI->GetSelectedString().end());

	Ptr<AMaterial> pMtrl = FIND(AMaterial, key);

	GetTarget()->MeshRender()->SetMaterial(pMtrl);
}
