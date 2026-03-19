#include "pch.h"
#include "FlipbookRenderUI.h"



FlipbookRenderUI::FlipbookRenderUI()
	: ComponentUI(COMPONENT_TYPE::FLIPBOOK_RENDER, "FlipbookRenderUI")
{
}

FlipbookRenderUI::~FlipbookRenderUI()
{
}

void FlipbookRenderUI::Tick_UI()
{
	OutputTitle("FlipbookRender");

	Ptr<CFlipbookRender> pFlipbookRender = GetTarget()->FlipbookRender();

	// ========
	// Flipbook
	// ========
	ImGui::Text("FlipBook");

	// FlipBook 텍스트에 마우스를 올리면 PLAYER_STATE_ID 전체 목록 표시
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Text("PLAYER_STATE_ID Reference:");
		ImGui::Separator();
		for (int i = 0; i < (int)PLAYER_STATE_ID::END; ++i)
		{
			ImGui::Text("[%d] %s", i, ToString((PLAYER_STATE_ID)i));
		}
		ImGui::EndTooltip();
	}

	vector<Ptr<AFlipbook>>& pVecFlipbook = pFlipbookRender->GetFlipbooks();

	// Vector에 있는 Flipbook들만 표시
	for (int i = 0; i < (int)pVecFlipbook.size(); ++i)
	{
		ImGui::Text("[%d]", i);
		ImGui::SameLine(100);

		// 해당 인덱스에 Flipbook이 있는지 확인
		if (pVecFlipbook[i] != nullptr)
		{
			// Flipbook이 있으면 이름 표시
			string FlipbookKey = string(pVecFlipbook[i]->GetKey().begin(), pVecFlipbook[i]->GetKey().end());
			ImGui::InputText(("##FlipbookName" + to_string(i)).c_str(), FlipbookKey.data(), FlipbookKey.length() + 1, ImGuiInputTextFlags_ReadOnly);

			// 삭제 버튼
			ImGui::SameLine();
			if (ImGui::Button(("X##Delete" + to_string(i)).c_str()))
			{
				pVecFlipbook[i] = nullptr;
			}
		}
		else
		{
			ImGui::Text("None");
		}
	}

	// 빈 공간에 드롭하면 vector 끝에 순서대로 추가
	ImGui::Spacing();
	ImGui::Text("Drag Flipbook here to add to end of list");
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");
		if (PayLoad)
		{
			DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
			Ptr<Asset> pAsset = (Asset*)data;
			if (ASSET_TYPE::FLIPBOOK == pAsset->GetType())
			{
				pFlipbookRender->AddFlipbook((AFlipbook*)pAsset.Get());
			}
		}
		ImGui::EndDragDropTarget();
	}
}
