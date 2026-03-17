#include "pch.h"
#include "SpriteUI.h"

#include "PathMgr.h"
#include "AssetMgr.h"
#include "ASprite.h"

SpriteUI::SpriteUI()
	: AssetUI(ASSET_TYPE::SPRITE)
	, m_PrevSprite(nullptr)
{
	memset(m_NameBuffer, 0, sizeof(m_NameBuffer));
}

SpriteUI::~SpriteUI()
{
}

void SpriteUI::Tick_UI()
{
	OutputTitle();

	Ptr<ASprite> pSprite = (ASprite*)GetTargetAsset().Get();

	// Sprite가 변경되었을 때만 버퍼 업데이트
	if (m_PrevSprite != pSprite.Get())
	{
		m_PrevSprite = pSprite.Get();
		string Key = string(pSprite->GetKey().begin(), pSprite->GetKey().end());
		strcpy_s(m_NameBuffer, sizeof(m_NameBuffer), Key.c_str());
	}

	string Key = string(pSprite->GetKey().begin(), pSprite->GetKey().end());

	ImGui::Text("Name");
	ImGui::SameLine(120);
	ImGui::InputText("##SpriteName", m_NameBuffer, sizeof(m_NameBuffer));

	// 이름이 변경되었는지 확인
	bool NameChanged = (Key != m_NameBuffer);
	if (NameChanged)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "(Modified)");
	}

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

	// Atlas 텍스처 선택 UI
	ImGui::Text("Atlas Texture");
	ImGui::SameLine(120);

	// Combo Box로 Texture 선택
	if (ImGui::BeginCombo("##AtlasTexture", AtlasName.c_str()))
	{
		// "None" 옵션 추가
		if (ImGui::Selectable("None", !IsAtlas))
		{
			pSprite->SetAtlas(nullptr);
		}

		// 현재 로드된 모든 Texture 목록 가져오기
		vector<wstring> vecTexNames;
		AssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);

		for (const auto& texName : vecTexNames)
		{
			string texNameStr = string(texName.begin(), texName.end());
			bool isSelected = (IsAtlas && texName == pAtlas->GetKey());

			if (ImGui::Selectable(texNameStr.c_str(), isSelected))
			{
				// 선택된 Texture를 Atlas로 설정
				Ptr<ATexture> pNewAtlas = AssetMgr::GetInst()->FindAsset<ATexture>(texName);
				pSprite->SetAtlas(pNewAtlas);
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

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

		if (IsAtlas)
		{
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
	}
	ImGui::EndDisabled();


	// Save Button
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	if (ImGui::Button("Save##SpriteSaveBtn"))
	{
		// char* → string → wstring 안전한 변환
		string fileNameStr(m_NameBuffer);
		wstring FileName(fileNameStr.begin(), fileNameStr.end());

		wstring FilePath = CONTENT_PATH;
		FilePath += L"Sprite\\";
		FilePath += FileName;
		FilePath += L".sprite";

		if (pSprite->Save(FilePath) == S_OK)
		{
			// 저장 성공 메시지
			ImGui::OpenPopup("Save Success");
		}
	}

	// 저장 성공 팝업
	if (ImGui::BeginPopupModal("Save Success", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Sprite saved successfully!");
		ImGui::Text("File: Sprite/%s.sprite", m_NameBuffer);
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// 저장 후 AssetMgr에 새 이름으로 등록
			string fileNameStr(m_NameBuffer);
			wstring newKey(fileNameStr.begin(), fileNameStr.end());
			AssetMgr::GetInst()->AddAsset(newKey, pSprite.Get());

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
