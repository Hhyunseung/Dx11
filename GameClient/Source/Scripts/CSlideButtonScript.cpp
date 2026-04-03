#include "pch.h"
#include "CSlideButtonScript.h"

#include "KeyMgr.h"

CSlideButtonScript::CSlideButtonScript()
	:CUIButtonScript(SCRIPT_TYPE::SLIDEBUTTONSCRIPT)
{
}
 
CSlideButtonScript::~CSlideButtonScript()
{
}


void CSlideButtonScript::Tick()
{
	CUIButtonScript::Tick();

	if (KEY_TAP(KEY::SPACE))
	{
		SetPressed(false);
		return;
	}
}

void CSlideButtonScript::OnButtonDown()
{
	SetPressed(true);
}

void CSlideButtonScript::OnButtonUp()
{
	SetPressed(false);
}
