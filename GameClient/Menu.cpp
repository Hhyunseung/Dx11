#include "pch.h"
#include "Menu.h"

#include "EditorMgr.h"

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::Tick()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Level Save")) 
			{

			}

			if (ImGui::MenuItem("Level Load"))
			{

			}


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			bool ShowDemo = EditorMgr::GetInst()->IsShowDemo();

			/// ShowDemo 쪽 인자... 체크 표시
			if (ImGui::MenuItem("Demo", nullptr, &ShowDemo, true))
			{
				/// 버튼을 눌러서 여기 들어오는것이니까 ShowDemo 가 바뀌어져 있다
				EditorMgr::GetInst()->ShowDemo(ShowDemo);
			}

			Ptr<EditorUI> pInspector = EditorMgr::GetInst()->FindUI("Inspector");
			bool InspectorActive = pInspector->IsActive();

			if (ImGui::MenuItem("Inspector", nullptr, &InspectorActive))
			{
				pInspector->SetActive(InspectorActive);
			}

			Ptr<EditorUI> pOutliner = EditorMgr::GetInst()->FindUI("Outliner");
			bool OutlinerActive = pOutliner->IsActive();

			if (ImGui::MenuItem("Outliner", nullptr, &OutlinerActive))
			{
				pOutliner->SetActive(OutlinerActive);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Menu::Tick_UI()
{
}
