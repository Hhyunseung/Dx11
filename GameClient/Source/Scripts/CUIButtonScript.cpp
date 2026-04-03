#include "pch.h"
#include "CUIButtonScript.h"

#include "Device.h"
#include "KeyMgr.h"
#include "GameObject.h"

#include "AMaterial.h"
#include "CMeshRender.h"
#include "CTransform.h"

CUIButtonScript::CUIButtonScript(SCRIPT_TYPE _Type)
	: CScript(_Type)
	, m_IsPressed(false)
	, m_PressDuration(0.f)
	, m_Size(Vec2(100.f, 50.f))
{
}

CUIButtonScript::CUIButtonScript()
	: CScript(SCRIPT_TYPE::UIBUTTONSCRIPT)
	, m_IsPressed(false)
	, m_PressDuration(0.f)
	, m_Size(Vec2(100.f, 50.f))
{
}

CUIButtonScript::~CUIButtonScript()
{
}

void CUIButtonScript::SetPressed(bool _Pressed)
{
	CMeshRender* pMR = MeshRender();
	if (pMR == nullptr)
		return;

	Ptr<AMaterial> pMtrl = pMR->GetMaterial();
	if (pMtrl == nullptr)
		return;

	if (_Pressed)
		pMtrl->SetScalar(INT_0, 1); // Pressed
	else
		pMtrl->SetScalar(INT_0, 0); // Normal
}

bool CUIButtonScript::IsMouseOver()
{
	Vec2 mousePos = KeyMgr::GetInst()->GetMousePos();
	Vec2 resolution = Device::GetInst()->GetRenderResolution();

	// 화면 좌표 → UI 좌표 변환
	float x = mousePos.x - resolution.x * 0.5f;
	float y = -(mousePos.y - resolution.y * 0.5f);

	Vec2 mouseWorld = Vec2(x, y);

	Vec3 objPos3 = Transform()->GetRelativePos();
	Vec2 objPos = Vec2(objPos3.x, objPos3.y);

	Vec2 half = m_Size * 0.5f;

	if (mouseWorld.x < objPos.x - half.x) return false;
	if (mouseWorld.x > objPos.x + half.x) return false;
	if (mouseWorld.y < objPos.y - half.y) return false;
	if (mouseWorld.y > objPos.y + half.y) return false;

	return true;
}

void CUIButtonScript::Tick()
{
	bool mouseOver = IsMouseOver();

	if (mouseOver && KEY_TAP(KEY::LBTN))
	{
		SetPressed(true);
		OnButtonDown();
	}

	if (m_IsPressed && KEY_PRESSED(KEY::LBTN))
	{
		SetPressed(true);
	}

	if (m_IsPressed && KEY_RELEASED(KEY::LBTN))
	{
		SetPressed(false);

		if (mouseOver)
		{
			OnButtonClick();
		}

		OnButtonUp();
	}
}
