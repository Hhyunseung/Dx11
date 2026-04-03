#include "pch.h"
#include "CSlideButtonScript.h"

#include "KeyMgr.h"

CSlideButtonScript::CSlideButtonScript()
	: CUIButtonScript(SCRIPT_TYPE::SLIDEBUTTONSCRIPT)
	, m_MouseHolding(false)
{
}
 
CSlideButtonScript::~CSlideButtonScript()
{
}


void CSlideButtonScript::Tick()
{
	CUIButtonScript::Tick();

	// 키보드로 슬라이드키를 누르고 있는 동안에도 눌린 이미지 표시
	SetPressed(m_MouseHolding || KEY_PRESSED(KEY::DOWN));
}

void CSlideButtonScript::OnButtonDown()
{
	m_MouseHolding = true;
	SetPressed(true);
}

void CSlideButtonScript::OnButtonUp()
{
	m_MouseHolding = false;
	SetPressed(false);
}
