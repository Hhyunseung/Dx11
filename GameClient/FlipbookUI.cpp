#include "pch.h"
#include "FlipbookUI.h"

#include "PathMgr.h"
#include "AssetMgr.h"

FlipbookUI::FlipbookUI()
	: AssetUI(ASSET_TYPE::FLIPBOOK)
	, m_PrevFlipbook(nullptr)
	, m_AccTime(0.f)
	, m_CurFrame(0)
	, m_FPS(10.f)
	, m_Play(true)
{
	memset(m_NameBuffer, 0, sizeof(m_NameBuffer));
}

FlipbookUI::~FlipbookUI()
{
}

void FlipbookUI::Tick_UI()
{
	OutputTitle();

	Ptr<AFlipbook> pFlipbook = (AFlipbook*)GetTargetAsset().Get();
	if (nullptr == pFlipbook)
		return;

	// Flipbook이 변경되었을 때만 버퍼 업데이트
	if (m_PrevFlipbook != pFlipbook.Get())
	{
		m_PrevFlipbook = pFlipbook.Get();
		string Key = string(pFlipbook->GetKey().begin(), pFlipbook->GetKey().end());
		strcpy_s(m_NameBuffer, sizeof(m_NameBuffer), Key.c_str());
	}

	string Key = string(pFlipbook->GetKey().begin(), pFlipbook->GetKey().end());

	ImGui::Text("Name");
	ImGui::SameLine(120);
	ImGui::InputText("##FlipbookName", m_NameBuffer, sizeof(m_NameBuffer));

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

	// Sprite 목록 편집
	SpriteList(pFlipbook);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();

	// 미리보기
	Preview(pFlipbook);

	// Save Button
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	if (ImGui::Button("Save##FlipbookSaveBtn"))
	{
		wstring FileName = wstring(m_NameBuffer, m_NameBuffer + strlen(m_NameBuffer));
		wstring FilePath = CONTENT_PATH;
		FilePath += L"Flipbook\\";
		FilePath += FileName;
		FilePath += L".flip";

		if (pFlipbook->Save(FilePath) == S_OK)
		{
			ImGui::OpenPopup("Save Success");
		}
	}

	// 저장 성공 팝업
	if (ImGui::BeginPopupModal("Save Success", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Flipbook saved successfully!");
		ImGui::Text("File: Flipbook/%s.flip", m_NameBuffer);
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			// 저장 후 AssetMgr에 새 이름으로 등록
			string fileNameStr(m_NameBuffer);
			wstring newKey(fileNameStr.begin(), fileNameStr.end());
			AssetMgr::GetInst()->AddAsset(newKey, pFlipbook.Get());

			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void FlipbookUI::Preview(Ptr<AFlipbook> pFlipbook)
{
	int FrameCount = pFlipbook->GetSpriteCount();

	if (FrameCount == 0)
		return;

	m_CurFrame %= FrameCount;

	if (m_Play)
	{
		m_AccTime += ImGui::GetIO().DeltaTime;

		float frameTime = 1.f / m_FPS;

		if (m_AccTime >= frameTime)
		{
			m_CurFrame++;
			m_AccTime = 0.f;

			if (m_CurFrame >= FrameCount)
				m_CurFrame = 0;
		}
	}

	Ptr<ASprite> pSprite = pFlipbook->GetSprite(m_CurFrame);

	if (nullptr == pSprite)
		return;

	Ptr<ATexture> pAtlas = pSprite->GetAtlas();

	Vec2 leftTop = pSprite->GetLeftTopUV();
	Vec2 slice = pSprite->GetSliceUV();

	Vec2 uv0 = leftTop;
	Vec2 uv1 = Vec2(leftTop.x + slice.x, leftTop.y + slice.y);

	float width = slice.x * pAtlas->GetWidth();
	float height = slice.y * pAtlas->GetHeight();

	float maxSize = 300.f;

	if (width > maxSize || height > maxSize)
	{
		float Scale = min(maxSize / width, maxSize / height);
		width *= Scale;
		height *= Scale;
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Text("Preview");

	ImGui::Image((void*)pAtlas->GetSRV().Get(), ImVec2(width, height), ImVec2(uv0.x, uv0.y), ImVec2(uv1.x, uv1.y));

	ImGui::Spacing();

	// UI 컨트롤
	if (ImGui::Button(m_Play ? "Pause" : "Play"))
	{
		m_Play = !m_Play;
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{
		m_CurFrame = 0;
	}

	ImGui::SliderFloat("FPS", &m_FPS, 1.f, 30.f);
}

void FlipbookUI::SpriteList(Ptr<AFlipbook> pFlipbook)
{
	ImGui::Text("Sprite List");
	ImGui::Spacing();

	int FrameCount = pFlipbook->GetSpriteCount();

	// 현재 Sprite 목록 표시
	if (ImGui::BeginChild("SpriteListChild", ImVec2(0, 200), true))
	{
		for (int i = 0; i < FrameCount; ++i)
		{
			Ptr<ASprite> pSprite = pFlipbook->GetSprite(i);
			string spriteName = pSprite.Get() ? 
				string(pSprite->GetKey().begin(), pSprite->GetKey().end()) : "None";

			ImGui::PushID(i);

			// 프레임 번호 표시
			ImGui::Text("[%d]", i);
			ImGui::SameLine();

			// Sprite 이름 표시
			ImGui::Text("%s", spriteName.c_str());
			ImGui::SameLine();

			// 삭제 버튼
			if (ImGui::Button("X"))
			{
				// Sprite 제거 (벡터에서 삭제)
				vector<Ptr<ASprite>> tempVec;
				for (int j = 0; j < FrameCount; ++j)
				{
					if (j != i)
						tempVec.push_back(pFlipbook->GetSprite(j));
				}

				// 벡터 재구성 (AFlipbook에 Clear 함수가 없으므로)
				// 직접 접근은 불가능하므로 Save/Load 또는 AssetMgr을 통한 재생성 필요
				// 여기서는 nullptr로 설정
				pFlipbook->SetSprite(i, nullptr);
			}

			ImGui::PopID();
		}
	}
	ImGui::EndChild();

	ImGui::Spacing();

	// Sprite 추가 UI
	ImGui::Text("Add Sprite");
	ImGui::SameLine();

	// Sprite 선택 드롭다운
	static int selectedSpriteIdx = 0;
	static vector<wstring> vecSpriteNames;

	if (ImGui::Button("Refresh Sprite List"))
	{
		vecSpriteNames.clear();
		AssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecSpriteNames);
		selectedSpriteIdx = 0;
	}

	if (vecSpriteNames.empty())
	{
		vecSpriteNames.clear();
		AssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecSpriteNames);
	}

	if (!vecSpriteNames.empty())
	{
		string currentSpriteName = string(vecSpriteNames[selectedSpriteIdx].begin(), 
										  vecSpriteNames[selectedSpriteIdx].end());

		if (ImGui::BeginCombo("##SpriteSelector", currentSpriteName.c_str()))
		{
			for (int i = 0; i < vecSpriteNames.size(); ++i)
			{
				string spriteName = string(vecSpriteNames[i].begin(), vecSpriteNames[i].end());
				bool isSelected = (selectedSpriteIdx == i);

				if (ImGui::Selectable(spriteName.c_str(), isSelected))
				{
					selectedSpriteIdx = i;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();

		// 추가 버튼
		if (ImGui::Button("Add"))
		{
			Ptr<ASprite> pNewSprite = AssetMgr::GetInst()->FindAsset<ASprite>(vecSpriteNames[selectedSpriteIdx]);
			if (pNewSprite.Get())
			{
				pFlipbook->AddSprite(pNewSprite);
			}
		}
	}
}
