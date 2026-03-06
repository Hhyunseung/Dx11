#include "pch.h"
#include "SpriteUI.h"

#include "PathMgr.h"
#include "ASprite.h"

SpriteUI::SpriteUI()
	: AssetUI(ASSET_TYPE::SPRITE)
{
}

SpriteUI::~SpriteUI()
{
}

void SpriteUI::Tick_UI()
{
	OutputTitle();

	Ptr<ASprite> pSprite = (ASprite*)GetTargetAsset().Get();

	string Key = string(pSprite->GetKey().begin(), pSprite->GetKey().end());

	ImGui::Text("Name");
	ImGui::SameLine(120);
	ImGui::InputText("##SpriteName", Key.data(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();

	// Sprite 에 Atlas 텍스쳐가 세팅되어있는지 확인
	Ptr<ATexture> pAtlas = pSprite->GetAtlas();
	bool IsAtlas = pAtlas.Get();

	// Atlas 이름
	string AtlasName = "None";
	if (IsAtlas)
		AtlasName = string(pAtlas->GetKey().begin(), pAtlas->GetKey().end());

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();


	ImGui::Text("Atlas Name");
	ImGui::SameLine(120);
	ImGui::InputText("##AtlasName", AtlasName.data(), AtlasName.length() + 1, ImGuiInputTextFlags_ReadOnly);

	// Sprite UV 정보
	Vec2 LeftTopUV = pSprite->GetLeftTopUV();
	Vec2 SliceUV = pSprite->GetSliceUV();
	Vec2 BackgroundUV = pSprite->GetBackgroundUV();
	Vec2 OffsetUV = pSprite->GetOffsetUV();

	ImGui::Text("LeftTop");
	ImGui::BeginDisabled(!IsAtlas);
	{
		if (IsAtlas)
		{
			int pixel[2] = { LeftTopUV.x * (float)pAtlas->GetWidth(),
							 LeftTopUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputInt2("##LeftTop", pixel))
			{
				pSprite->SetLeftTopUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth()
										 , (float)pixel[1] / (float)pAtlas->GetHeight()));
			}
		}
		else
		{
			int pixel[2] = { 0,0 };
			ImGui::InputInt2("##LeftTop", pixel);
		}
	}
	ImGui::EndDisabled();

	ImGui::Text("Slice");
	ImGui::BeginDisabled(!IsAtlas);
	{
		if (IsAtlas)
		{
			int pixel[2] = { SliceUV.x * (float)pAtlas->GetWidth()
						   , SliceUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputInt2("##Slice", pixel))
			{
				pSprite->SetSliceUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth()
									   , (float)pixel[1] / (float)pAtlas->GetHeight()));
			}
		}
		else
		{
			int pixel[2] = { 0, 0 };
			ImGui::InputInt2("##Slice", pixel);
		}
	}
	ImGui::EndDisabled();

	ImGui::Text("Background");
	ImGui::BeginDisabled(!IsAtlas);
	{
		if (IsAtlas)
		{
			int pixel[2] = { BackgroundUV.x * (float)pAtlas->GetWidth()
						  ,  BackgroundUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputInt2("##Background", pixel))
			{
				pSprite->SetBackgroundUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth()
					, (float)pixel[1] / (float)pAtlas->GetHeight()));
			}
		}
		else
		{
			int pixel[2] = { 0, 0 };
			ImGui::InputInt2("##Background", pixel);
		}
	}
	ImGui::EndDisabled();

	ImGui::Text("Offset");
	ImGui::BeginDisabled(!IsAtlas);
	{
		if (IsAtlas)
		{
			int pixel[2] = { OffsetUV.x * (float)pAtlas->GetWidth()
						  ,  OffsetUV.y * (float)pAtlas->GetHeight() };

			if (ImGui::InputInt2("##Offset", pixel))
			{
				pSprite->SetOffsetUV(Vec2((float)pixel[0] / (float)pAtlas->GetWidth()
					, (float)pixel[1] / (float)pAtlas->GetHeight()));
			}
		}
		else
		{
			int pixel[2] = { 0, 0 };
			ImGui::InputInt2("##Offset", pixel);
		}
	}
	ImGui::EndDisabled();

	// Sprite 미리보기 (Atlas 텍스쳐가 세팅되어있을 때만)
	ImGui::BeginDisabled(!IsAtlas);
	{
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Preview");

		// UV 좌표 계산
		Vec2 uv0 = LeftTopUV;
		Vec2 uv1 = Vec2(LeftTopUV.x + SliceUV.x, LeftTopUV.y + SliceUV.y);

		// 미리보기 크기 (원본 비율 유지하면서 적절한 크기로 표시)
		float previewWidth = SliceUV.x * (float)pAtlas->GetWidth();
		float previewHeight = SliceUV.y * (float)pAtlas->GetHeight();

		// 너무 크면 스케일 다운
		float maxPreviewSize = 300.0f;
		if (previewWidth > maxPreviewSize || previewHeight > maxPreviewSize)
		{
			float scale = min(maxPreviewSize / previewWidth, maxPreviewSize / previewHeight);
			previewWidth *= scale;
			previewHeight *= scale;
		}

		// 이미지 표시
		ImGui::Image((void*)pAtlas->GetSRV().Get(),
			ImVec2(previewWidth, previewHeight),
			ImVec2(uv0.x, uv0.y),
			ImVec2(uv1.x, uv1.y));
	}
	ImGui::EndDisabled();


	// Save Button
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	if (ImGui::Button("Save##MtrlSaveBtn"))
	{
		wstring FilePath = CONTENT_PATH + pSprite->GetKey();
		pSprite->Save(FilePath);
	}
}
