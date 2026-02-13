#include "pch.h"
#include "EditorUI.h"

#include "EditorMgr.h"


EditorUI::EditorUI(const string& _Name)
	: m_UIName(_Name)
	, m_Active(true)
	, m_Parent(nullptr)
{
}

EditorUI::~EditorUI()
{
}

void EditorUI::Tick()
{
	if (nullptr == m_Parent)
	{
		bool Active = m_Active;

		ImGui::Begin(GetUIName().c_str(), &Active);

		if (m_Active != Active)
		{
			SetActive(Active);
		}

		CheckFocus();

		Tick_UI();

		for (size_t i = 0; i < m_ChildUI.size(); ++i)
		{
			if (m_ChildUI[i]->IsActive())
			{
				m_ChildUI[i]->Tick();
				ImGui::Separator(); /// 자식 UI 들이 구분되도록 구분선 삽입
			}
		}

		ImGui::End();
	}

	else
	{
		ImGui::BeginChild(m_UIName.c_str(), m_SizeAsChild);

		CheckFocus();

		Tick_UI();

		for (size_t i = 0; i < m_ChildUI.size(); ++i)
		{
			if (m_ChildUI[i]->IsActive())
			{
				m_ChildUI[i]->Tick();
				ImGui::Separator(); /// 자식 UI 들이 구분되도록 구분선 삽입
			}
		}

		ImGui::EndChild();
	}
}

void EditorUI::CheckFocus()
{
	/// Begin 과 End 사이에서 체크해야한다
	if (ImGui::IsWindowFocused())
		EditorMgr::GetInst()->RegisterFocusedUI(this);
}


Vec2::operator ImVec2() const
{
	return ImVec2(x, y);
}

Vec4::operator ImVec4() const
{
	return ImVec4(x, y, z, w);
}