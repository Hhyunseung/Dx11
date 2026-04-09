#include "pch.h"
#include "CPauseButtonScript.h"

#include "GamePlayMgr.h"

CPauseButtonScript::CPauseButtonScript()
	:CUIButtonScript(SCRIPT_TYPE::PAUSEBUTTONSCRIPT)
{
}

CPauseButtonScript::~CPauseButtonScript()
{
}

void CPauseButtonScript::OnButtonClick()
{
	GamePlayMgr::GetInst()->PauseGame();
}

