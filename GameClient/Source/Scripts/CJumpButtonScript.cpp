#include "pch.h"
#include "CJumpButtonScript.h"

#include "GamePlayMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

CJumpButtonScript::CJumpButtonScript()
{
}

CJumpButtonScript::~CJumpButtonScript()
{
}


void CJumpButtonScript::Tick()
{
	CUIButtonScript::Tick();

	if (KEY_TAP(KEY::SPACE))
	{
		m_PressDuration = 0.15f;
	}

	if (m_PressDuration > 0.f)
	{
		m_PressDuration -= DT;
		SetPressed(true);
	}
	else
	{
		if (!GetPressed())
			SetPressed(false);
	}
}

void CJumpButtonScript::OnButtonClick()
{
	CPlayerScript* pPlayer = GamePlayMgr::GetInst()->GetPlayerScript();
	if (pPlayer == nullptr)
		return;

	pPlayer->RequestJump();

	m_PressDuration = 0.15f;
}

