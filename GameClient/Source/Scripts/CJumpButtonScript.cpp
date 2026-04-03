#include "pch.h"
#include "CJumpButtonScript.h"

#include "GamePlayMgr.h"
#include "KeyMgr.h"

CJumpButtonScript::CJumpButtonScript()
	: CUIButtonScript(SCRIPT_TYPE::JUMPBUTTONSCRIPT)
	, m_Clicked(false)
{
}

CJumpButtonScript::~CJumpButtonScript()
{
}


void CJumpButtonScript::Tick()
{
	CUIButtonScript::Tick();
}

void CJumpButtonScript::OnButtonDown()
{
	m_Clicked = true;
}

void CJumpButtonScript::OnButtonUp()
{

}


bool CJumpButtonScript::ConsumeClick()
{
	if (!m_Clicked)
		return false;

	m_Clicked = false;
	return true;
}