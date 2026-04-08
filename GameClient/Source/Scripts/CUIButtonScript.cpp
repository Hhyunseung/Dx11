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
	, m_Size(Vec2(300.f, 300.f))
	, m_IsMouseHolding(false)
{
}

CUIButtonScript::CUIButtonScript()
	: CScript(SCRIPT_TYPE::UIBUTTONSCRIPT)
	, m_IsPressed(false)
	, m_Size(Vec2(300.f, 300.f))
	, m_IsMouseHolding(false)
{
}

CUIButtonScript::~CUIButtonScript()
{
}

void CUIButtonScript::SetPressed(bool _Pressed)
{
	m_IsPressed = _Pressed;

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

	float x = mousePos.x - resolution.x * 0.5f;
	float y = -(mousePos.y - resolution.y * 0.5f);

	Vec2 mouseWorld(x, y);

	Vec3 objPos3 = Transform()->GetRelativePos();
	Vec2 objPos(objPos3.x, objPos3.y);

	Vec3 scale3 = Transform()->GetRelativeScale();
	Vec2 size(fabsf(scale3.x), fabsf(scale3.y));
	Vec2 half = size * 0.5f;

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
		m_IsMouseHolding = true;
		OnButtonDown();
	}

	if (m_IsMouseHolding && KEY_RELEASED(KEY::LBTN))
	{
		m_IsMouseHolding = false;

		if (mouseOver)
		{
			OnButtonClick();
		}

		OnButtonUp();
	}
}