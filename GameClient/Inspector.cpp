#include "pch.h"
#include "Inspector.h"

#include "LevelMgr.h"

#include "TransformUI.h"
#include "Collider2DUI.h"
#include "CameraUI.h"
#include "Light2DUI.h"

Inspector::Inspector()
	: EditorUI("Inspector")
{
	CreateChildUI();

	SetTargetObject(LevelMgr::GetInst()->FindObjectByName(L"BackGround1"));
}

Inspector::~Inspector()
{

}

void Inspector::SetTargetObject(Ptr<GameObject> _Object)
{
	m_TargetObject = _Object;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComUI[i] == nullptr)
			continue;

		m_arrComUI[i]->SetTarget(m_TargetObject);
	}
}

void Inspector::Tick_UI()
{
	if (m_TargetObject == nullptr)
		return;

	wstring Name = m_TargetObject->GetName();
	string strName(string(Name.begin(), Name.end()));

	if (strName.empty())
		strName = "No Name";

	ImGui::Button(strName.c_str());

	ImGui::Separator();
}
