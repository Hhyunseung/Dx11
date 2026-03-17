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

	vector<Ptr<AFlipbook>>& pVecFlipbook = pFlipbookRender->GetFlipbooks();

	// PLAYER_STATE_ID::END까지 모든 상태를 순회
	for (int i = 0; i < (int)PLAYER_STATE_ID::END; ++i)
	{
		ImGui::Text("[%s]", ToString((PLAYER_STATE_ID)i));
		ImGui::SameLine(100);

		// 해당 인덱스에 Flipbook이 있는지 확인 (범위 체크 포함)
		if (i < pVecFlipbook.size() && pVecFlipbook[i] != nullptr)
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
			// Flipbook이 없으면 "None" 표시
			ImGui::Text("None");

			// None 위에 Drag-Drop 타겟 설정 (새로 추가)
			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");
				if (PayLoad)
				{
					DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
					Ptr<Asset> pAsset = (Asset*)data;
					if (ASSET_TYPE::FLIPBOOK == pAsset->GetType())
					{
						pFlipbookRender->SetFlipbook(i, (AFlipbook*)pAsset.Get());
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
	}

	// 스킬?
	//// 빈 공간에 드롭하면 새로 추가
	//if (ImGui::BeginDragDropTarget())
	//{
	//	const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentUI");
	//	if (PayLoad)
	//	{
	//		DWORD_PTR data = *((DWORD_PTR*)PayLoad->Data);
	//		Ptr<Asset> pAsset = (Asset*)data;
	//		if (ASSET_TYPE::FLIPBOOK == pAsset->GetType())
	//		{
	//			pFlipbookRender->AddFlipbook((AFlipbook*)pAsset.Get());
	//		}
	//	}
	//	ImGui::EndDragDropTarget();
	//}
}
