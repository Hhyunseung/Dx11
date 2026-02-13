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
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSizeAsChild(Vec2(0.f, 250.f));
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM].Get());

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSizeAsChild(Vec2(0.f, 350.f));
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D].Get());

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetSizeAsChild(Vec2(0.f, 350.f));
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA].Get());

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSizeAsChild(Vec2(0.f, 350.f));
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D].Get());

	SetTargetObject(LevelMgr::GetInst()->FindObjectByName(L"Light_1"));
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
	ImGui::Button(strName.c_str());

	ImGui::Separator();
}
