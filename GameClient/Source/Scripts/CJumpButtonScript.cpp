#include "pch.h"
#include "CJumpButtonScript.h"

#include "GamePlayMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

CJumpButtonScript::CJumpButtonScript()
	: CUIButtonScript(SCRIPT_TYPE::JUMPBUTTONSCRIPT)
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
		OnButtonClick();
	}

	if (GetPressed() || KEY_PRESSED(KEY::SPACE))
	{
		SetPressed(true);
	}
	else
	{
		SetPressed(false);
	}
}

void CJumpButtonScript::OnButtonDown()
{
	CPlayerScript* pPlayer = GamePlayMgr::GetInst()->GetPlayerScript();
	if (pPlayer == nullptr)
		return;

	pPlayer->RequestJump();
}

