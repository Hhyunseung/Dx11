#include "pch.h"
#include "FlipbookUI.h"

#include "PathMgr.h"

FlipbookUI::FlipbookUI()
	: AssetUI(ASSET_TYPE::FLIPBOOK)
	, m_AccTime(0.f)
	, m_CurFrame (0)
	, m_FPS(10.f)
	, m_Play(true)
{
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

	string Key = string(pFlipbook->GetKey().begin(), pFlipbook->GetKey().end());

	ImGui::Text("Name");
	ImGui::SameLine(120);
	ImGui::InputText("##FlipbookName", Key.data(), Key.length() + 1, ImGuiInputTextFlags_ReadOnly);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();

	Preview(pFlipbook);

	// Save Button
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
	if (ImGui::Button("Save##FlipbookSaveBtn"))
	{
		wstring FilePath = CONTENT_PATH + pFlipbook->GetKey();
		pFlipbook->Save(FilePath);
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
