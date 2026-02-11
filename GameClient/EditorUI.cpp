#include "pch.h"
#include "EditorUI.h"

EditorUI::EditorUI()
	: m_Active(true)
{
}

EditorUI::~EditorUI()
{
}

void EditorUI::Tick()
{
	ImGui::Begin(GetUIName().c_str(), &m_Active);

	Tick_UI();

	ImGui::End();
}
