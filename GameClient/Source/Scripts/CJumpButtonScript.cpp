#include "pch.h"
#include "CJumpButtonScript.h"

#include "GamePlayMgr.h"
#include "KeyMgr.h"

CJumpButtonScript::CJumpButtonScript()
	: CUIButtonScript(SCRIPT_TYPE::JUMPBUTTONSCRIPT)
	, m_MouseHolding(false)
{
}

CJumpButtonScript::~CJumpButtonScript()
{
}


void CJumpButtonScript::Tick()
{
	CUIButtonScript::Tick();

	bool bKeyboardJumpHold = KEY_PRESSED(KEY::SPACE);

	SetPressed(m_MouseHolding || bKeyboardJumpHold);
}

void CJumpButtonScript::OnButtonDown()
{
	m_MouseHolding = true;
	m_Clicked = true;
	SetPressed(true);
}

void CJumpButtonScript::OnButtonUp()
{
	m_MouseHolding = false;
	SetPressed(false);
}


bool CJumpButtonScript::ConsumeClick()
{
	if (!m_Clicked)
		return false;

	m_Clicked = false;
	return true;
}